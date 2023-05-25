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

#define RECEIVER_GPIO                (uint_fast8_t)(8)
#define LEFT_CHANNEL                      (uint8_t)(13)
#define RIGHT_CHANNEL                     (uint8_t)(5)
#define UD_POTENTIOMETER_GPIO             (uint8_t)(10)
#define INC_POTENTIOMETER_GPIO            (uint8_t)(9)

#define DEBUG_RX                            uint8_t(7)  /* Software serial debugger GPIO RX pin*/
#define DEBUG_TX                            uint8_t(6)  /* Software serial debugger GPIO TX pin*/

#define CHANNEL_SELECTED                (uint8_t)(0x0)
#define CHANNEL_UNSELECTED              (uint8_t)(0x1)

#define STD_ON                                     (1)
#define STD_OFF                                    (0)

#define DEBUG_PRINTER                        (STD_ON)
#define SOFTWARE_SERIAL_DEBUG                (STD_OFF)
#define DEBUG_IR_FULL_INFO                   (STD_OFF)
#define AVR_WDT_ENABLE                       (STD_ON)
#define INIT_POTENTIOMETERS_WITH_EEPROM_VAL  (STD_ON)

#define ENABLE_TEST_MODE                     (STD_OFF)  /* test EEPROM write operation duration with test GPIO*/

#if(ENABLE_TEST_MODE == STD_ON)
#define TEST_GPIO                         (uint8_t)(2)  /* test EEPROM write operation duration with test GPIO*/
#endif

#define POTENTIOMETER_LOW_BOUNDRY         (uint8_t)(1)  /* 3 KOhm */
#define POTENTIOMETER_HIGH_BOUNDRY       (uint8_t)(10)  /*30 KOhm with step of 3 KOhm*/

#define POTETNIOMETER_RESET_VALUE         (uint8_t)(5)
#define DELAY_PERIOD                   (uint16_t)(100)
#define WDT_TRIGGER_TIME                      WDTO_4S

/*Parameters to be stored in the EEPROM memory*/

struct ChannelsConfiguration
{
  uint8_t channel_left_resistance_value;
  uint8_t channel_right_resistance_value;

  void Reset()
  {
    channel_left_resistance_value = POTETNIOMETER_RESET_VALUE;
    channel_right_resistance_value = POTETNIOMETER_RESET_VALUE;
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
    LEFT_CHANNEL_SELECT_F     = 1 << 0,
    RIGHT_CHANNEL_SELECT_F    = 1 << 1,
    CHANNEL_SELECTION_IDLE_F  = 1 << 2
};

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

#endif

