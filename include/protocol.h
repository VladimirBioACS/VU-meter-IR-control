#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <Arduino.h>

/*Address value*/
#define SELECT_RIGHT_CHANNEL_CMD_ADDR                  (uint16_t)(0x0)
#define SELECT_LEFT_CHANNEL_CMD_ADDR                   (uint16_t)(0x0)
#define INCREASE_POTENTIOMETER_VAL_CMD_ADDR            (uint16_t)(0x0)
#define DECREASE_POTENTIOMETER_VAL_CMD_ADDR            (uint16_t)(0x0)
#define COMMIT_CHANGES_CMD_ADDR                        (uint16_t)(0x0)

/*CMD value*/
#define SELECT_RIGHT_CHANNEL_CMD_C                      (uint16_t)(0x46)
#define SELECT_LEFT_CHANNEL_CMD_C                       (uint16_t)(0x45)
#define INCREASE_POTENTIOMETER_VAL_CMD_C                (uint16_t)(0x18)
#define DECREASE_POTENTIOMETER_VAL_CMD_C                (uint16_t)(0x52)
#define COMMIT_CHANGES_CMD_C                            (uint16_t)(0x1C)

/*Raw data value*/
#define SELECT_RIGHT_CHANNEL_CMD_RAW                    (uint32_t)(0xB946FF00)
#define SELECT_LEFT_CHANNEL_CMD_RAW                     (uint32_t)(0xBA45FF00)
#define INCREASE_POTENTIOMETER_VAL_CMD_RAW              (uint32_t)(0xE718FF00)
#define DECREASE_POTENTIOMETER_VAL_CMD_RAW              (uint32_t)(0xAD52FF00)
#define COMMIT_CHANGES_CMD_RAW                          (uint32_t)(0xE31CFF00)

#endif