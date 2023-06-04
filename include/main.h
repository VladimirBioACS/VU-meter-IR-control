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
/*------------------------------------------------------Configs------------------------------------------------------*/
/*********************************************************************************************************************/

#define BAUDRATE                            (unsigned long)(115200)

#define RECEIVER_GPIO                       (uint_fast8_t)(8)
#define LEFT_CHANNEL                        (uint8_t)(13)
#define RIGHT_CHANNEL                       (uint8_t)(5)
#define UD_POTENTIOMETER_GPIO               (uint8_t)(10)
#define INC_POTENTIOMETER_GPIO              (uint8_t)(9)

#define DEBUG_RX                            (uint8_t)(7)              /* Software serial debugger GPIO RX pin*/
#define DEBUG_TX                            (uint8_t)(6)              /* Software serial debugger GPIO TX pin*/

#define CHANNEL_SELECTED                    (uint8_t)(0x0)
#define CHANNEL_UNSELECTED                  (uint8_t)(0x1)

#define STD_ON                              (1)
#define STD_OFF                             (0)

#define DEBUG_PRINTER                       (STD_OFF)
#define SOFTWARE_SERIAL_DEBUG               (STD_OFF)
#define DEBUG_IR_FULL_INFO                  (STD_OFF)
#define AVR_WDT_ENABLE                      (STD_ON)
#define INIT_POTENTIOMETERS_WITH_EEPROM_VAL (STD_ON)
#define EEPROM_CHECK_TASK_ENABLE            (STD_ON)
#define ARDUINO_PROFILER                    (STD_OFF)

#define POTENTIOMETER_LOW_BOUNDRY           (uint8_t)(1)              /* 3 KOhm */
#define POTENTIOMETER_HIGH_BOUNDRY          (uint8_t)(14)             /*42 KOhm with step of 3 KOhm (14 * 3 = 42)*/

#define POTETNIOMETER_RESET_VALUE           (uint8_t)(5)
#define DELAY_PERIOD                        (int)(100)                /* 100ms delay for non-blocking timer */
#define DELAY_EEPROM_CHECK                  (1000UL * 60 * 5)         /* delay 5 minutes */
#define WDT_TRIGGER_TIME                    WDTO_4S

/*********************************************************************************************************************/
/*--------------------------------------------------------PVs--------------------------------------------------------*/
/*********************************************************************************************************************/

/*Parameters to be stored in the EEPROM memory*/
struct ChannelsConfiguration 
{
  uint8_t channel_left_step_value;
  uint8_t channel_right_step_value;

  void Reset()
  {
    channel_left_step_value = POTETNIOMETER_RESET_VALUE;
    channel_right_step_value = POTETNIOMETER_RESET_VALUE;
  }
};

/*enumeration for potentiometers CS line selection*/
enum channelsState 
{
  LEFT_CHANNEL_SELECT,
  RIGHT_CHANNEL_SELECT,
  RELEASE_CHANNELS_CS_LINES,
  SELECT_ALL_CS_CHANNELS
};

/*enumeration for channel selection flags*/
enum channelSelectFlags 
{
  LEFT_CHANNEL_SELECT_F = 1 << 0,
  RIGHT_CHANNEL_SELECT_F = 1 << 1,
  CHANNEL_SELECTION_IDLE_F = 1 << 2
};

#endif
