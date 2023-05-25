#include "cs_port_LL.h"

/**
* @brief Function implements the low level GPIO port initialization for CS lines
* @param argument: None
* @retval None
*/
void CSportInit(void)
{
    /* Config ports as OUTPUT*/
    DDRC = PORTC_BITMASK;

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
    switch (state)
    {
    /*Left channel selected*/
    case 0:
        /* code */
        PORTC = PORTC | B10000000; 
        break;
    
    /*Right channel selected*/
    case 1:
        PORTC = PORTC | B01000000; 
    
    /*Release CS lines*/
    case 2:
        PORTC &= !B11000000;
        break;
    
    default:
        break;
    }
}
