/**
**********************************************************************************************************************
*    @file           : main.h
*    @brief          : main program header
**********************************************************************************************************************
*    @author     Volodymyr Noha
*    @license    MIT (see License.txt)
*
*    @description:
*    This is a simple firmware for the Audio VU meter. The firmware implements the IR remote control of the device.
*    Header contains all function definitions and main program configs
*
*    @section  HISTORY
*    v1.0  - First version
*
**********************************************************************************************************************
*/

#ifndef MAIN_H_
#define MAIN_H_

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

#define BAUDRATE               (unsigned long)(115200)
#define RECEIVER_GPIO                              (9)
#define LEFT_CHANNEL                      (uint8_t)(8)
#define RIGHT_CHANNEL                     (uint8_t)(7)
#define UD_POTENTIOMETER_GPIO             (uint8_t)(6)
#define INC_POTENTIOMETER_GPIO            (uint8_t)(5)

#define CHANNEL_SELECTED                (uint8_t)(0x0)
#define CHANNEL_UNSELECTED              (uint8_t)(0x1)
#define STD_ON                                     (1)
#define STD_OFF                                    (0)

#define DEBUG_PRINTER                        (STD_ON)
#define DEBUG_IR_FULL_INFO                   (STD_OFF)
#define AVR_WDT_ENABLE                        (STD_ON)

#define POTENTIOMETER_LOW_BOUNDRY         (uint8_t)(1)
#define POTENTIOMETER_HIGH_BOUNDRY       (uint8_t)(30)

/*Parameters to be stored in the EEPROM memory*/
struct ChannelsConfiguration
{
  uint8_t channel_left_resistance_value;
  uint8_t channel_right_resistance_value;

  void Reset()
  {
    channel_left_resistance_value = POTENTIOMETER_LOW_BOUNDRY;
    channel_right_resistance_value = POTENTIOMETER_LOW_BOUNDRY;
  }
};


enum channelsState
{
    LEFT_CHANNEL_SELECT,
    RIGHT_CHANNEL_SELECT,
    RELEASE_CHANNELS_CS_LINES
};

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/

static void storeEepromConfig(uint8_t left_channel_value, uint8_t right_channel_value);
static void potentiometerChannelSelect(int option);
static void irDataReceive();

#if (DEBUG_PRINTER == STD_ON && DEBUG_IR_FULL_INFO == STD_ON)
static void irReceiveCmdInfo();
#endif

#endif