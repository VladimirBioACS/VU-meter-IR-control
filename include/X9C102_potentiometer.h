#ifndef DIG_POTENTIOMETER_H
#define DIG_POTENTIOMETER_H

#include <Arduino.h>

#define MAX_RESISTANCE (30)
#define MIN_RESISTANCE (1)

#define POTENTIOMETER_RESOLUTION (100)
#define POTENTIOMETER_TICK (1)

enum potentiometer_direction
{
    DIRECTION_UP = 1,
    DIRECTION_DOWN = 2
};

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
    void potentiometerSetVal(uint8_t val, uint8_t dir);
    void setValueLeftChannel(uint8_t val);
};

#endif