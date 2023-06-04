/**
**********************************************************************************************************************
*    @file           : platform.h
*    @brief          : platform.h header file body
**********************************************************************************************************************
*    @author     Volodymyr Noha
*    @license    MIT (see License.txt)
*
*    @description:
*    This header file contains platform specific information
*
*    @section  HISTORY
*    v1.0  - First version
*
**********************************************************************************************************************
*/


#ifndef PLATFORM_H_
#define PLATFORM_H_

#define FIRMWARE_VERSION        "1.0.0"
#define DEVICE_DESCRIPTION      "VU-meter digital controller"
#define MCU                     "ATmega32U 5V"

#define EEPROM_VOLUME           int(4096)
#define FLASH_VOLUME            int(28672)
#define RAM_VOLUME              int(2560)

#endif