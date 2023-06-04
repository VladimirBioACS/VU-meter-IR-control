/**
**********************************************************************************************************************
*    @file           : protocol.h
*    @brief          : protocol.h header file body
**********************************************************************************************************************
*    @author     Volodymyr Noha
*    @license    MIT (see License.txt)
*
*    @description:
*    This header file contains IR protocol CMD`s
*
*    @section  HISTORY
*    v1.0  - First version
*
**********************************************************************************************************************
*/

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <Arduino.h>

/*Address value*/
#define SELECT_RIGHT_CHANNEL_CMD_ADDR                  (uint16_t)(0x6B86)
#define SELECT_LEFT_CHANNEL_CMD_ADDR                   (uint16_t)(0x6B86)
#define INCREASE_VU_VALUE_CMD_ADDR                     (uint16_t)(0x6B86)
#define DECREASE_VU_VALUE_CMD_ADDR                     (uint16_t)(0x6B86)
#define COMMIT_CHANGES_CMD_ADDR                        (uint16_t)(0x6B86)
#define PRINT_DEBUG_INFO_CMD_ADDR                      (uint16_t)(0x6B86)
#define FACTORY_RESET_VU_VAL_CMD_ADDR                  (uint16_t)(0x6B86)

/*CMD value*/
#define SELECT_RIGHT_CHANNEL_CMD_C                     (uint16_t)(0x2)
#define SELECT_LEFT_CHANNEL_CMD_C                      (uint16_t)(0x1)
#define INCREASE_VU_VALUE_CMD_C                        (uint16_t)(0x1A)
#define DECREASE_VU_VALUE_CMD_C                        (uint16_t)(0x1E)
#define COMMIT_CHANGES_CMD_C                           (uint16_t)(0x12)
#define PRINT_DEBUG_INFO_CMD_C                         (uint16_t)(0xD)
#define FACTORY_RESET_VU_VAL_CMD_C                     (uint16_t)(0xE)

/*Raw data value*/
#define SELECT_RIGHT_CHANNEL_CMD_RAW                   (uint32_t)(0xFD026B86)
#define SELECT_LEFT_CHANNEL_CMD_RAW                    (uint32_t)(0xFE016B86)
#define INCREASE_VU_VALUE_CMD_RAW                      (uint32_t)(0xE51A6B86)
#define DECREASE_VU_VALUE_CMD_RAW                      (uint32_t)(0xE11E6B86)
#define COMMIT_CHANGES_CMD_RAW                         (uint32_t)(0xED126B86)
#define PRINT_DEBUG_INFO_CMD_RAW                       (uint32_t)(0xF20D6B86)
#define FACTORY_RESET_VU_VAL_CMD_RAW                   (uint32_t)(0xF10E6B86)

#endif