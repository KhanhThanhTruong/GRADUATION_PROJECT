#ifndef SD_MYBUILT_H
#define SD_MYBUILT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>

String pushData(const char *filename, StaticJsonDocument<200> js);

#endif