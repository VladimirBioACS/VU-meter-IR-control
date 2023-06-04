/**
**********************************************************************************************************************
*    @file           : cs_port_LL.h
*    @brief          : cs_port_LL.h header file body
**********************************************************************************************************************
*    @author     Volodymyr Noha
*    @license    MIT (see License.txt)
*
*    @description:
*    Implements the low level port intercation for X9C102 potentiometer CS line control
*
*    @section  HISTORY
*    v1.0  - First version
*
**********************************************************************************************************************
*/


#ifndef GPIO_LL_H_
#define GPIO_LL_H_

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/

#include <Arduino.h>

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

#define PORTB_BITMASK       (uint8_t)((1 << DDB5) | (1 << DDB5))
#define PORTC_BITMASK       (uint8_t)((1 << DDC6) | (1 << DDC7))

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/

void CSportInit(void);
void CSportSet(uint8_t state);

#endif