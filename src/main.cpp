/**
 __      ___    _        __  __ ______ _______ ______ _____
\ \    / / |  | |      |  \/  |  ____|__   __|  ____|  __ \
 \ \  / /| |  | |______| \  / | |__     | |  | |__  | |__) |
  \ \/ / | |  | |______| |\/| |  __|    | |  |  __| |  _  /
   \  /  | |__| |      | |  | | |____   | |  | |____| | \ \
    \/    \____/       |_|  |_|______|  |_|  |______|_|  \_\
**********************************************************************************************************************
*    @file           : main.c
*    @brief          : main program body
**********************************************************************************************************************
*    @author     Volodymyr Noha
*    @license    MIT (see License.txt)
*
*    @description:
*    This is a simple firmware for the Audio VU meter. The firmware implements the IR remote control of the device
*
*    @section  HISTORY
*    v1.0  - First version
*
**********************************************************************************************************************
*/

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/

#include <Arduino.h>
#include "EEPROMStore.h"
#include "X9C102_potentiometer.h"
#include "IRremote.h"
#include "cs_port_LL.h"
#include "protocol.h"

#if (DEBUG_PRINTER == STD_ON || SOFTWARE_SERIAL_DEBUG == STD_ON)
#include "platform.h"
#endif

#if (AVR_WDT_ENABLE == STD_ON)
#include "avr/wdt.h"
#endif

#include "main.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

/* Serial log setup*/
#if (DEBUG_PRINTER == STD_ON && SOFTWARE_SERIAL_DEBUG == STD_OFF)

#define DEBUG_SETUP(baudrate) Serial.begin(baudrate)
#define DEBUG(string) Serial.print(string)
#define DEBUG_NL(string_nln) Serial.println(string_nln)

#pragma message("Hardware serial debug enabled")

#elif (DEBUG_PRINTER == STD_ON && SOFTWARE_SERIAL_DEBUG == STD_ON)

#include "SoftwareSerial.h"

SoftwareSerial softSerial(DEBUG_RX, DEBUG_TX);

#define DEBUG_SETUP(baudrate) softSerial.begin(baudrate)
#define DEBUG(string) softSerial.print(string)
#define DEBUG_NL(string_nln) softSerial.println(string_nln)

#pragma message("Software serial debug enabled")

#else

#define DEBUG_SETUP(baudrate)
#define DEBUG(string)
#define DEBUG_NL(string_nln)
#pragma message("Debug disabled")

#endif

/*********************************************************************************************************************/
/*------------------------------------------------------Classes------------------------------------------------------*/
/*********************************************************************************************************************/

IRrecv irreciver(RECEIVER_GPIO);
X9C102_potentiometer potentiometer(UD_POTENTIOMETER_GPIO, INC_POTENTIOMETER_GPIO);

EEPROMStore<ChannelsConfiguration> Configuration;

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/

static void potentiometerChannelSelect(int option);
static bool storeEepromConfig(uint8_t left_channel_value, uint8_t right_channel_value);
static void irDataReceive(void);

#if(DEBUG_PRINTER == STD_ON || SOFTWARE_SERIAL_DEBUG == STD_ON)
static void showSystemInfo(void);
#endif

#if (DEBUG_PRINTER == STD_ON && DEBUG_IR_FULL_INFO == STD_ON)
static void irReceiveCmdInfo();
#endif

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/

#if (DEBUG_PRINTER == STD_ON || SOFTWARE_SERIAL_DEBUG == STD_ON)
/**
 * @brief This function prints system info via UART
 * @param argument: None
 * @retval None
 */
static void showSystemInfo(void)
{

  DEBUG_NL("=====PLATFORM INFO START=====");

  DEBUG("[MCU]: ");
  DEBUG_NL(MCU);

  DEBUG("[FIRMWARE VERSION]: ");
  DEBUG_NL(FIRMWARE_VERSION);

  DEBUG("[DEVICE DESCRIPTION]: ");
  DEBUG_NL(DEVICE_DESCRIPTION);

  DEBUG("[EEPROM VOLUME]: ");
  DEBUG(EEPROM_VOLUME);
  DEBUG_NL(" bytes");

  DEBUG("[FLASH VOLUME]: ");
  DEBUG(FLASH_VOLUME);
  DEBUG_NL(" bytes");

  DEBUG("[RAM VOLUME]: ");
  DEBUG(RAM_VOLUME);
  DEBUG_NL(" bytes");

  DEBUG("[POTENTIOMETER MAX RESISTANCE]: ");
  DEBUG(MAX_RESISTANCE);
  DEBUG_NL(" KOhm");

  DEBUG("[POTENTIOMETER MIN RESISTANCE]: ");
  DEBUG(MIN_RESISTANCE);
  DEBUG_NL(" KOhm");

#if (AVR_WDT_ENABLE == STD_ON)
  DEBUG_NL("[OPTION]: Watchdog timer: [ENABLED]");
  DEBUG("[OPTION]: Watchdog config: ");
  DEBUG_NL(WDT_TRIGGER_TIME);
#endif

#if (DEBUG_PRINTER == STD_ON)
  DEBUG_NL("[OPTION]: USB Debug printer: [ENABLED]");
#endif

#if (SOFTWARE_SERIAL_DEBUG == STD_ON)
  DEBUG_NL("[OPTION]: Software serial Debug printer: [ENABLED]");
#endif

#if (INIT_POTENTIOMETERS_WITH_EEPROM_VAL == STD_ON)
  DEBUG_NL("[OPTION]: Potentiometer initialization from EEPROM: [ENABLED]");

  DEBUG("[OPTION]: Current EEPROM value for Left channel: ");
  DEBUG_NL(Configuration.Data.channel_left_resistance_value);

  DEBUG("[OPTION]: Current EEPROM value for Right channel: ");
  DEBUG_NL(Configuration.Data.channel_right_resistance_value);
#endif

  DEBUG_NL("=====PLATFORM INFO START=====");
  DEBUG_NL("");
}

#endif

#if (DEBUG_PRINTER == STD_ON && DEBUG_IR_FULL_INFO == STD_ON)
/**
 * @brief Function implements the received IR data parsing. Mainly used for the debug
 * @param argument: None
 * @retval None
 */
static void irReceiveCmdInfo()
{
  if (irreciver.decode())
  {
    DEBUG_NL("Basic info:");
    irreciver.printIRResultMinimal(&Serial);
    DEBUG_NL();
    DEBUG_NL("Basic raw format:");
    irreciver.printIRResultRawFormatted(&Serial);
    DEBUG_NL();
    DEBUG_NL("Result as a C variable:");
    irreciver.printIRResultAsCVariables(&Serial);
    DEBUG_NL();
  }
  irreciver.resume();
}
#endif

/**
 * @brief Function implements the EEPROM config storage and returns status after EEPROM write
 * @param argument: uint8_t left_channel_value, uint8_t right_channel_value
 * @retval None
 */
static bool storeEepromConfig(uint8_t left_channel_value, uint8_t right_channel_value)
{
  Configuration.Data.channel_left_resistance_value = left_channel_value;
  Configuration.Data.channel_right_resistance_value = right_channel_value;

  bool eeprom_status_f = Configuration.Save();

  return eeprom_status_f;
}

/**
 * @brief Function implements the selection mechanism for left and Right channel potentiometers
 * @param argument: int option
 * @retval None
 */
static void potentiometerChannelSelect(int option)
{
  switch (option)
  {
  case LEFT_CHANNEL_SELECT:
    CSportSet(LEFT_CHANNEL_SELECT);
    break;

  case RIGHT_CHANNEL_SELECT:
    CSportSet(RIGHT_CHANNEL_SELECT);
    break;

  case RELEASE_CHANNELS_CS_LINES: /*release the potentiometer CS line to avoid sporadic value change*/
    CSportSet(RELEASE_CHANNELS_CS_LINES);
    break;

  case SELECT_ALL_CS_CHANNELS:
    CSportSet(SELECT_ALL_CS_CHANNELS);
    break;

  default:
    break;
  }
}

/**
 * @brief Function implements the reception of the IR CMD and contains its processing logic
 * @param argument: None
 * @retval None
 */
static void irDataReceive(void)
{
  static uint8_t right_channel_value = Configuration.Data.channel_right_resistance_value;
  static uint8_t left_channel_value = Configuration.Data.channel_left_resistance_value;

  static uint8_t channel_select_f = CHANNEL_SELECTION_IDLE_F;

  /* should be tested */
  if (right_channel_value < POTENTIOMETER_LOW_BOUNDRY)
  {
    right_channel_value = POTENTIOMETER_LOW_BOUNDRY;
  }

  if (left_channel_value < POTENTIOMETER_LOW_BOUNDRY)
  {
    left_channel_value = POTENTIOMETER_LOW_BOUNDRY;
  }

  if (irreciver.decode())
  {
    if (irreciver.decodedIRData.protocol != UNKNOWN)
    {
      uint32_t received_cmd = irreciver.decodedIRData.decodedRawData;

      switch (received_cmd)
      {
      case SELECT_RIGHT_CHANNEL_CMD_RAW:
        DEBUG_NL("[CMD received]: Right channel selected");

        channel_select_f = RIGHT_CHANNEL_SELECT_F;
        potentiometerChannelSelect(RIGHT_CHANNEL_SELECT);

        break;

      case SELECT_LEFT_CHANNEL_CMD_RAW:
        DEBUG_NL("[CMD received]: Left channel selected");

        channel_select_f = LEFT_CHANNEL_SELECT_F;
        potentiometerChannelSelect(LEFT_CHANNEL_SELECT);

        break;

      case COMMIT_CHANGES_CMD_RAW:
        DEBUG_NL("[CMD received]: Changes commited");

        potentiometerChannelSelect(RELEASE_CHANNELS_CS_LINES);
        channel_select_f = CHANNEL_SELECTION_IDLE_F;

        DEBUG_NL("Stored values: ");

        DEBUG("Right channel: ");
        DEBUG(right_channel_value);
        DEBUG_NL("");

        DEBUG("Left channel: ");
        DEBUG(left_channel_value);
        DEBUG_NL("");

        if (storeEepromConfig(left_channel_value, right_channel_value))
        {
          DEBUG_NL("Configuration stored in eeprom");
          DEBUG_NL("EEPROM storage content: ");

          DEBUG("Right channel: ");
          DEBUG(Configuration.Data.channel_right_resistance_value);
          DEBUG_NL("");

          DEBUG("Left channel: ");
          DEBUG(Configuration.Data.channel_left_resistance_value);
          DEBUG_NL("");
        }

        else
        {
          DEBUG_NL("EEPROM data did not changed");
        }

        break;

      case INCREASE_POTENTIOMETER_VAL_CMD_RAW:
        DEBUG_NL("[CMD received]: value UP");

        if (channel_select_f == LEFT_CHANNEL_SELECT_F)
        {
          ++left_channel_value;

          if (left_channel_value <= POTENTIOMETER_HIGH_BOUNDRY)
          {
            potentiometer.potentiometerSetVal(left_channel_value, DIRECTION_DOWN);
            DEBUG_NL(left_channel_value);
          }

          else
          {
            left_channel_value = POTENTIOMETER_HIGH_BOUNDRY;
          }
        }

        if (channel_select_f == RIGHT_CHANNEL_SELECT_F)
        {
          ++right_channel_value;

          if (right_channel_value <= POTENTIOMETER_HIGH_BOUNDRY)
          {
            potentiometer.potentiometerSetVal(right_channel_value, DIRECTION_UP);
            DEBUG_NL(right_channel_value);
          }

          else
          {
            right_channel_value = POTENTIOMETER_HIGH_BOUNDRY;
          }
        }
        else
        {
          // empty
        }

        break;

      case DECREASE_POTENTIOMETER_VAL_CMD_RAW:
        DEBUG_NL("[CMD received]: value DOWN");

        if (channel_select_f == LEFT_CHANNEL_SELECT_F)
        {
          --left_channel_value;

          if (left_channel_value >= POTENTIOMETER_LOW_BOUNDRY)
          {
            potentiometer.potentiometerSetVal(left_channel_value, DIRECTION_DOWN);
            DEBUG_NL(left_channel_value);
          }

          else
          {
            left_channel_value = POTENTIOMETER_LOW_BOUNDRY;
          }
        }

        if (channel_select_f == RIGHT_CHANNEL_SELECT_F)
        {
          --right_channel_value;

          if (right_channel_value >= POTENTIOMETER_LOW_BOUNDRY)
          {
            potentiometer.potentiometerSetVal(right_channel_value, DIRECTION_UP);
            DEBUG_NL(right_channel_value);
          }

          else
          {
            right_channel_value = POTENTIOMETER_LOW_BOUNDRY;
          }
        }
        else
        {
          // empty
        }

        break;

/*Debug info*/
#if (DEBUG_PRINTER == STD_ON || SOFTWARE_SERIAL_DEBUG == STD_ON)
      case PRINT_DEBUG_INFO_CMD_RAW:
        showSystemInfo();
        break;
#endif

      default:
        DEBUG_NL("[CMD received]: Unknown command");
        break;
      }
    }

    else
    {
      DEBUG_NL("Unknown protocol");
    }
  }

/**
 * EEPROM check task
 * This task checks the EEPROM values every 5 minutes
 * This task should re-write EEPROM with actual potentiometer values
 * in case if EEPROM will not be updated by pressing "OK" button
*/
#if(EEPROM_CHECK_TASK_ENABLE == STD_ON)
  static uint32_t old_time_eeprom_check_value = millis();

  if ((millis() - old_time_eeprom_check_value) > DELAY_EEPROM_CHECK) /* Timer for non-blocking delay */
  {
    if (storeEepromConfig(left_channel_value, right_channel_value))
    {
      DEBUG_NL("EEPROM Check task");
      DEBUG_NL("EEPROM stored");
      DEBUG_NL("EEPROM storage content: ");

      DEBUG("Right channel value: ");
      DEBUG(Configuration.Data.channel_right_resistance_value);
      DEBUG_NL("");

      DEBUG("Left channel value: ");
      DEBUG(Configuration.Data.channel_left_resistance_value);
      DEBUG_NL("");
    }

    else
    {
      DEBUG_NL("EEPROM Check task");
      DEBUG_NL("EEPROM data did not changed");
    }

    old_time_eeprom_check_value = millis();
  }
#endif

  irreciver.resume();
}

/**
 * @brief Main setup function
 * @param argument: None
 * @retval None
 */
void setup()
{
/* WDG initialization */
#if (AVR_WDT_ENABLE == STD_ON)
  wdt_enable(WDT_TRIGGER_TIME); /* WDTO_4S => 4 second timeout. */
#endif

  DEBUG_SETUP(BAUDRATE);

  /* GPIO initialization */
  CSportInit();

  /* External devices initialization */
  irreciver.enableIRIn();
  potentiometer.potentiometerInit();

#if (INIT_POTENTIOMETERS_WITH_EEPROM_VAL == STD_ON)

  potentiometerChannelSelect(LEFT_CHANNEL_SELECT);
  potentiometer.potentiometerSetVal(Configuration.Data.channel_left_resistance_value, DIRECTION_DOWN);

  potentiometerChannelSelect(RIGHT_CHANNEL_SELECT);
  potentiometer.potentiometerSetVal(Configuration.Data.channel_right_resistance_value, DIRECTION_UP);

#endif

  potentiometerChannelSelect(RELEASE_CHANNELS_CS_LINES);
}

/**
 * @brief Main superloop function
 * @param argument: None
 * @retval None
 */
void loop()
{
  /* Main loop */
  static uint32_t old_tim_value = millis();

  if ((millis() - old_tim_value) > DELAY_PERIOD) /* Timer for non-blocking delay */
  {
#if (DEBUG_PRINTER == STD_ON && DEBUG_IR_FULL_INFO == STD_ON)
    irReceiveCmdInfo();
#else
    irDataReceive();
#endif

    old_tim_value = millis();
  }

/* WDG pet */
#if (AVR_WDT_ENABLE == STD_ON)
  wdt_reset();
#endif
}