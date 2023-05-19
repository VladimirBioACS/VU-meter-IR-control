#ifndef DIG_POTENTIOMETER_H
#define DIG_POTENTIOMETER_H

#include <Arduino.h>

#define POTENTIOMETER_RESOLUTION 30
#define POTENTIOMETER_TICK 1

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
        void potentiometerInit();
        void potentiometerSetVal(uint8_t val);
    };

#endif