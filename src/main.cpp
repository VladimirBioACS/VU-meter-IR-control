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
#include "main.h"
#include "X9C102_potentiometer.h"
#include "IRremote.h"
#include "protocol.h"

#if (AVR_WDT_ENABLE == STD_ON)
  #include "avr\wdt.h"
#endif

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

/* Serial log setup*/
#if(DEBUG_PRINTER == STD_ON && SOFTWARE_SERIAL_DEBUG == STD_OFF)

  #define DEBUG_SETUP(baudrate)     Serial.begin(baudrate)
  #define DEBUG(string)             Serial.print(string)
  #define DEBUG_NL(string_nln)      Serial.println(string_nln)

  #pragma message ("Hardware serial debug enabled")

#elif(DEBUG_PRINTER == STD_ON && SOFTWARE_SERIAL_DEBUG == STD_ON)

  #include "SoftwareSerial.h"

  #define DEBUG_RX                  uint8_t(3)
  #define DEBUG_TX                  uint8_t(4)

  SoftwareSerial softSerial(DEBUG_RX, DEBUG_TX);

  #define DEBUG_SETUP(baudrate)     softSerial.begin(baudrate)
  #define DEBUG(string)             softSerial.print(string)
  #define DEBUG_NL(string_nln)      softSerial.println(string_nln)

  #pragma message ("Software serial debug enabled")

#else

  #define DEBUG_SETUP(baudrate)
  #define DEBUG(string)
  #define DEBUG_NL(string_nln)
  #pragma message ("Debug disabled")

#endif

/*********************************************************************************************************************/
/*------------------------------------------------------Variables----------------------------------------------------*/
/*********************************************************************************************************************/

unsigned long tim_value_now;

/*********************************************************************************************************************/
/*------------------------------------------------------Classes------------------------------------------------------*/
/*********************************************************************************************************************/

IRrecv irreciver (RECEIVER_GPIO);
X9C102_potentiometer potentiometer(UD_POTENTIOMETER_GPIO, INC_POTENTIOMETER_GPIO);
EEPROMStore<ChannelsConfiguration> Configuration;

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/

/**
* @brief Function implements the received IR data parsing. Mainly used for the debug
* @param argument: None
* @retval None
*/
#if (DEBUG_PRINTER == STD_ON && DEBUG_IR_FULL_INFO == STD_ON)
static void irReceiveCmdInfo()
{
  if(irreciver.decode())
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
* @brief Function implements the selection mechanism for left and right channel potentiometers
* @param argument: int option
* @retval None
*/
static void potentiometerChannelSelect(int option)
{
  switch (option)
  {
  case LEFT_CHANNEL_SELECT:

    digitalWrite(LEFT_CHANNEL, CHANNEL_SELECTED);
    digitalWrite(RIGHT_CHANNEL, CHANNEL_UNSELECTED);
    break;
  
  case RIGHT_CHANNEL_SELECT:
    digitalWrite(LEFT_CHANNEL, CHANNEL_UNSELECTED);
    digitalWrite(RIGHT_CHANNEL, CHANNEL_SELECTED);
    break;

  case RELEASE_CHANNELS_CS_LINES:       /*release the potentiometer CS line to avoid sporadic value change*/
    digitalWrite(LEFT_CHANNEL, CHANNEL_UNSELECTED);
    digitalWrite(RIGHT_CHANNEL, CHANNEL_UNSELECTED);
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

  static bool left_channel_select_f, right_channel_select_f = false;

  if(irreciver.decode())
  {
    if(irreciver.decodedIRData.protocol != UNKNOWN)
    {
      uint32_t received_cmd = irreciver.decodedIRData.decodedRawData;
      
      switch (received_cmd)
      {
      case SELECT_RIGHT_CHANNEL_CMD_RAW:
        DEBUG_NL("[CMD received]: Right channel selected");

        right_channel_select_f = true;
        left_channel_select_f = false;

        potentiometerChannelSelect(RIGHT_CHANNEL_SELECT);

        break;
      
      case SELECT_LEFT_CHANNEL_CMD_RAW:
        DEBUG_NL("[CMD received]: Left channel selected");

        right_channel_select_f = false;
        left_channel_select_f = true;

        potentiometerChannelSelect(LEFT_CHANNEL_SELECT);

        break;

      case COMMIT_CHANGES_CMD_RAW:
        DEBUG_NL("[CMD received]: Changes commited");

        potentiometerChannelSelect(RELEASE_CHANNELS_CS_LINES);

        DEBUG_NL("Stored values: ");

        DEBUG("Right channel: ");
        DEBUG(right_channel_value);
        DEBUG_NL("");

        DEBUG("Left channel: ");
        DEBUG(left_channel_value);
        DEBUG_NL("");

        if(storeEepromConfig(left_channel_value, right_channel_value))
        {
          DEBUG_NL("Configuration stored in eeprom");

          right_channel_select_f = false;
          left_channel_select_f = false;

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

        if(left_channel_select_f)
        {
          ++left_channel_value;
          
          if(left_channel_value <= POTENTIOMETER_HIGH_BOUNDRY)
          {
            potentiometer.potentiometerSetVal(left_channel_value);
            DEBUG_NL(left_channel_value);
          }
          else
          {
            left_channel_value = POTENTIOMETER_HIGH_BOUNDRY;
          }
        }

        if(right_channel_select_f)
        {
          ++right_channel_value;
          
          if(right_channel_value <= POTENTIOMETER_HIGH_BOUNDRY)
          {
            potentiometer.potentiometerSetVal(right_channel_value);
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

        if(left_channel_select_f)
        {
          --left_channel_value;
          
          if(left_channel_value >= POTENTIOMETER_LOW_BOUNDRY)
          {
            potentiometer.potentiometerSetVal(left_channel_value);
            DEBUG_NL(left_channel_value);
          }
          else
          {
            left_channel_value = POTENTIOMETER_LOW_BOUNDRY;
          }
        }

        if(right_channel_select_f)
        {
          --right_channel_value;
          
          if(right_channel_value >= POTENTIOMETER_LOW_BOUNDRY)
          {
            potentiometer.potentiometerSetVal(right_channel_value);
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
    wdt_enable(WDTO_4S);               /* WDTO_4S => 4 second timeout. */
  #endif

  DEBUG_SETUP(BAUDRATE);

  /* GPIO initialization */
  pinMode(LEFT_CHANNEL, OUTPUT);
  pinMode(RIGHT_CHANNEL, OUTPUT);
  potentiometerChannelSelect(RELEASE_CHANNELS_CS_LINES);

  /* External devices initialization */
  irreciver.enableIRIn();
  potentiometer.potentiometerInit();

  #if(INIT_POTENTIOMETERS_WITH_EEPROM_VAL == STD_ON)
    potentiometerChannelSelect(LEFT_CHANNEL_SELECT);
    potentiometer.potentiometerSetVal(Configuration.Data.channel_left_resistance_value);

    potentiometerChannelSelect(RIGHT_CHANNEL_SELECT);
    potentiometer.potentiometerSetVal(Configuration.Data.channel_right_resistance_value);

    potentiometerChannelSelect(RELEASE_CHANNELS_CS_LINES);
  #endif

  /* Timer value store */
  tim_value_now = millis();

  DEBUG_NL("==== System inited ====");
}

/**
* @brief Main superloop function
* @param argument: None
* @retval None
*/
void loop() 
{
  /* Main loop */
  if(millis() >= tim_value_now + DELAY_PERIOD)     /* Timer for non-blocking delay */
  {
    #if (DEBUG_PRINTER == STD_ON && DEBUG_IR_FULL_INFO == STD_ON)
      irReceiveCmdInfo();
    #else
      irDataReceive();
    #endif
  }

/* WDG pet */
#if (AVR_WDT_ENABLE == STD_ON)
    wdt_reset();
#endif
}