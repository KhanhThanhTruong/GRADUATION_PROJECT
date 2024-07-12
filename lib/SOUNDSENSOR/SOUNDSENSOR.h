#ifndef SOUNDSENSOR_H
#define SOUNDSENSOR_H

#include <Arduino.h>
#define SOUNDSENSOR_PIN 32
#define V_OFFSET 2.5
#define SENSITIVITY_MICROPHONE_DB -44
#define GAIN_MAX4466_DB 95
#define ALPHA_SOUND 0.1

float smoothedValue = 0;
float getSoundLevel()
{
    // int sensorValue = analogReadMilliVolts(SOUNDSENSOR_PIN) ; // Chuyển đổi giá trị ADC vào điện áp
    float sensorValue = analogRead(SOUNDSENSOR_PIN) * (3.3 / 4095.0); // Chuyển đổi giá trị ADC thành điện áp
    smoothedValue = ALPHA_SOUND * sensorValue + (1 - ALPHA_SOUND) * smoothedValue;
    return smoothedValue;
}

#endif