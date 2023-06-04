/**
**********************************************************************************************************************
*    @file           : X9C102_potentiometer.h
*    @brief          : X9C102_potentiometer.h header file body
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

#ifndef DIG_POTENTIOMETER_H
#define DIG_POTENTIOMETER_H

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/

#include <Arduino.h>

/*********************************************************************************************************************/
/*-----------------------------------------------------Constants-----------------------------------------------------*/
/*********************************************************************************************************************/

#define MAX_RESISTANCE (42)
#define MIN_RESISTANCE (1)

#define POTENTIOMETER_RESOLUTION (100)
#define POTENTIOMETER_TICK (1)

/*********************************************************************************************************************/
/*--------------------------------------------------------PVs--------------------------------------------------------*/
/*********************************************************************************************************************/

typedef enum
{
    DIRECTION_UP = 1,
    DIRECTION_DOWN = 2
} potentiometer_direction;

/*********************************************************************************************************************/
/*------------------------------------------------------Classes------------------------------------------------------*/
/*********************************************************************************************************************/

class X9C102_potentiometer 
{

protected:
    uint8_t _UD;
    uint8_t _INC;

private:
    void resetValue();
    void setValue(uint8_t val);

public:
    X9C102_potentiometer(uint8_t UD, uint8_t INC);
    void potentiometerInit(void);
    void potentiometerSetVal(uint8_t val, potentiometer_direction dir);
    void setValueLeftChannel(uint8_t val);
};

#endif