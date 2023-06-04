/**
**********************************************************************************************************************
*    @file           : cs_port_LL.cpp
*    @brief          : cs_port_LL.cpp program body
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

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/

#include "cs_port_LL.h"

/**
* @brief Function implements the low level GPIO port initialization for CS lines
* @param argument: None
* @retval None
*/
void CSportInit(void)
{
    /* Config ports as OUTPUT*/
    DDRC = DDRC | B11000000;

    /* Set ports initial value as 1 (no potentiometer selected)*/
    PORTC = PORTC | B11000000; // 7bit - PC7 - Right channel; 6bit - PC6 - Left channel
}

/**
* @brief Function implements the low level GPIO port manipulation for CS lines (set its state)
* @param argument: None
* @retval None
*/
void CSportSet(uint8_t state)
{
    switch (state) {
    /*Left channel selected*/
    case 0:
        /* code */
        PORTC =  B01000000; 
        break;
    
    /*Right channel selected*/
    case 1:
        PORTC =  B10000000; 
        break;
    
    /*Release CS lines*/
    case 2:
        PORTC =  B11000000;
        break;

    case 3:
        PORTC = B00000000; 
        break;
    
    default:
        break;
    }
}
