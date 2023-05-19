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
#if(DEBUG_PRINTER == STD_ON)

  #define DEBUG_SETUP(baudrate)     Serial.begin(baudrate)
  #define DEBUG(string)             Serial.print(string)
  #define DEBUG_NL(string_nln)      Serial.println(string_nln)

#else

  #define DEBUG_SETUP(baudrate)
  #define DEBUG(string)
  #define DEBUG_NL(string_nln)
  #pragma message ("Debug disabled")

#endif

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
* @param argument: Not used
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
* @brief Function implements the EEPROM config storage
* @param argument: uint8_t left_channel_value, uint8_t right_channel_value
* @retval None
*/
static void storeEepromConfig(uint8_t left_channel_value, uint8_t right_channel_value)
{
  Configuration.Data.channel_left_resistance_value = left_channel_value;
  Configuration.Data.channel_right_resistance_value = right_channel_value;

  Configuration.Save();
  DEBUG_NL("Configuration stored in eeprom");
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
static void irDataReceive()
{
  if(irreciver.decode())
  {
    if(irreciver.decodedIRData.protocol != UNKNOWN)
    {
      uint32_t received_cmd = irreciver.decodedIRData.decodedRawData;
      
      switch (received_cmd)
      {
      case SELECT_RIGHT_CHANNEL_CMD_RAW:
        DEBUG_NL("[CMD received]: Right channel selected");
        potentiometerChannelSelect(1);
        break;
      
      case SELECT_LEFT_CHANNEL_CMD_RAW:
        DEBUG_NL("[CMD received]: Left channel selected");
        potentiometerChannelSelect(0);
        break;

      case COMMIT_CHANGES_CMD_RAW:
        DEBUG_NL("[CMD received]: Changes commited");
        potentiometerChannelSelect(2);
        break;

      case INCREASE_POTENTIOMETER_VAL_CMD_RAW:
        DEBUG_NL("[CMD received]: value UP");
        break;

      case DECREASE_POTENTIOMETER_VAL_CMD_RAW:
        DEBUG_NL("[CMD received]: value DOWN");
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
  #if (AVR_WDT_ENABLE == STD_ON)
    wdt_enable(WDTO_4S);               // WDTO_4S => 4 second timeout.
  #endif

  DEBUG_SETUP(BAUDRATE);

  pinMode(LEFT_CHANNEL, OUTPUT);
  pinMode(RIGHT_CHANNEL, OUTPUT);

  potentiometerChannelSelect(RELEASE_CHANNELS_CS_LINES);

  irreciver.enableIRIn();
  potentiometer.potentiometerInit();

  DEBUG_NL("==== System inited ====");

}

/**
* @brief Main superloop function
* @param argument: None
* @retval None
*/
void loop() 
{
  #if (DEBUG_PRINTER == STD_ON && DEBUG_IR_FULL_INFO == STD_ON)
  irReceiveCmdInfo();
  #endif

  irDataReceive();
  
  #if (AVR_WDT_ENABLE == STD_ON)
    wdt_reset();
  #endif

  delay(500);
}