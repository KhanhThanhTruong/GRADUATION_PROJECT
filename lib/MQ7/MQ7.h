#ifndef MQ7_H
#define MQ7_H

#include <Arduino.h>
#define alpha1 -0.0445
#define alpha2 -0.0336
#define beta1 3.572
#define beta2 2.7
#define gama1 106.03
#define gama2 -1.492
#define VCC 5.0
class MQ7
{
    uint8_t _pin;
    float _concentration;

public:
    // Analog input pin
    MQ7(uint8_t pin)
    {
        _pin = pin;
        pinMode(_pin, INPUT);
    };
    float getConcentration(float temp = 25, float humidity = 85)
    {
        int voltage_adc = analogRead(_pin);
        float v_out = (voltage_adc * VCC) / 1023.0;
        float rs_divide_r0 = (VCC - v_out) / v_out;
        float k_rh_t = (alpha1 * humidity + beta1) * pow((temp + 15), (alpha2 * humidity + beta2));
        float _concentration = pow(rs_divide_r0 * k_rh_t, gama1) * gama2;
        return _concentration;
    };
};

#endif