#ifndef MQ7_H
#define MQ7_H

#include <Arduino.h>
#define alpha1 2.43
#define alpha2 0.097
#define beta1 0.99
#define beta2 0.242
#define gama1 106.03
#define gama2 -1.492
#define VCC 3.3
#define MQ7_PIN_AO 34
#define MQ7_PIN_DO 35
    float getConcentration(float temp = 25, float humidity = 85)
    {
        int voltage_adc = analogReadMilliVolts(MQ7_PIN_AO);
        // Serial.print("ADC: "); Serial.println(voltage_adc);
        float v_out = (float)voltage_adc/1000.0;
        // Serial.print("Vol: "); Serial.println(v_out);
        float rs_divide_r0 = (VCC - v_out) / v_out;
        // Serial.print("R: "); Serial.println(rs_divide_r0);
        float k_rh_t = (alpha1 - humidity/100 * beta1) * pow((temp + 15), (alpha2 * humidity/100- beta2));
        // Serial.print("k: "); Serial.println(k_rh_t);
        float _concentration = pow(rs_divide_r0 * k_rh_t, gama2) * gama1;
        return _concentration;
    };


#endif