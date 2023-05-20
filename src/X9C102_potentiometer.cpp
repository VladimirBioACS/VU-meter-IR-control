#include "X9C102_potentiometer.h"

void X9C102_potentiometer::resetValue()
{
    for (size_t i = 0; i < POTENTIOMETER_RESOLUTION; i++)
    {
        digitalWrite(_INC, 0x0);
        delayMicroseconds(POTENTIOMETER_TICK);
        digitalWrite(_INC, 0x1);
        delayMicroseconds(POTENTIOMETER_TICK);
    }
}


void X9C102_potentiometer::setValue(uint8_t val)
{
    for (size_t i = 0; i < val; i++)
    {
        digitalWrite(_INC, 0x0);
        delayMicroseconds(POTENTIOMETER_TICK);
        digitalWrite(_INC, 0x1);
        delayMicroseconds(POTENTIOMETER_TICK);
    }
}


X9C102_potentiometer::X9C102_potentiometer(uint8_t UD, uint8_t INC)
{
    _UD  =  UD;
    _INC =  INC;
}


void X9C102_potentiometer::potentiometerInit()
{
    pinMode(_UD,  0x1);
    pinMode(_INC, 0x1);
}


void X9C102_potentiometer::potentiometerSetVal(uint8_t val)
{
    // digitalWrite(_CS, 0x0);
    // digitalWrite(_UD, 0x0);
    // resetValue();
    digitalWrite(_UD, 0x1);
    setValue(val);
}