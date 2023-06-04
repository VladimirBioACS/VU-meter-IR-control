/**
**********************************************************************************************************************
*    @file           : X9C102_potentiometer.cpp
*    @brief          : X9C102_potentiometer.cpp program body
**********************************************************************************************************************
*    @author     Volodymyr Noha
*    @license    MIT (see License.txt)
*
*    @description:
*    Simple library for interaction with X9C102 potentiometers
*
*    @section  HISTORY
*    v1.0  - First version
*
**********************************************************************************************************************
*/

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/

#include "X9C102_potentiometer.h"

/**
 * @brief Constructor for X9C102_potentiometer object
 * @param argument: uint8_t UD, uint8_t INC
 * @retval None
 */
X9C102_potentiometer::X9C102_potentiometer(uint8_t UD, uint8_t INC)
{
    _UD = UD;
    _INC = INC;
}

/**
 * @brief Function implements the value reset of the X9C102 digital potentiometer
 * @param argument: None
 * @retval None
 */
void X9C102_potentiometer::resetValue()
{
    for (size_t i = 0; i < POTENTIOMETER_RESOLUTION; i++) {
        digitalWrite(_INC, 0x0);
        delayMicroseconds(POTENTIOMETER_TICK);
        digitalWrite(_INC, 0x1);
        delayMicroseconds(POTENTIOMETER_TICK);
    }
}

/**
 * @brief Function implements the value set of the X9C102 digital potentiometer
 * @param argument: uint8_t val
 * @retval None
 */
void X9C102_potentiometer::setValue(uint8_t val)
{
    for (size_t i = 0; i < val; i++) {
        digitalWrite(_INC, 0x0);
        delayMicroseconds(POTENTIOMETER_TICK);
        digitalWrite(_INC, 0x1);
        delayMicroseconds(POTENTIOMETER_TICK);
    }
}

/**
 * @brief Function implements the initialization of the X9C102 digital potentiometer
 * @param argument: None
 * @retval None
 */
void X9C102_potentiometer::potentiometerInit()
{
    pinMode(_UD, 0x1);
    pinMode(_INC, 0x1);
}

/**
 * @brief Function implements the public interface for setting the X9C102 digital potentiometer value
 * @param argument: uint8_t val, potentiometer_direction dir
 * @retval None
 */
void X9C102_potentiometer::potentiometerSetVal(uint8_t val, potentiometer_direction dir)
{
    switch (dir) {

    case DIRECTION_UP: /* for right channel*/
        digitalWrite(_UD, 0x0);
        resetValue();
        digitalWrite(_UD, 0x1);
        break;

    case DIRECTION_DOWN: /* for left channel*/
        digitalWrite(_UD, 0x1);
        resetValue();
        digitalWrite(_UD, 0x0);
        break;

    default:
        break;
    }

    setValue(val);
}