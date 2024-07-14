#ifndef SD_MYBUILT_H
#define SD_MYBUILT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>

#define SD_CS_PIN 0

namespace SD_JS

{
    // String JSON_TITLE[11]={"TEMPERATURE","HUMIDITY","LATITUDE","LONGITUDE","TIME","PM1_0","PM2_5","PM10","CO","POWER","NET_SERVICE"};
    enum JSON_COMPONENT
    {
        TEMPERATURE = 0,
        HUMIDITY = 1,
        LATITUDE = 2,
        LONGITUDE = 3,
        TIME = 4,
        PM1_0 = 5,
        PM2_5 = 6,
        PM10 = 7,
        CO = 8,
        POWER = 9,
        NET_SERVICE = 10
    };
    // Function init SD card
    String init(const int csPin = SD_CS_PIN);

    // Function push json data to file "filename" in SD card
    // File name must start with /
    String pushData(const char *filename, String js_str);
    String pushData(StaticJsonDocument<200> js);
    // Function read json data from file "filename" in SD card
    String getData(const char *filename, StaticJsonDocument<200> &js);

    // Function delete file "filename" in SD card
    String deleteFile(const char *filename);

    // Function delete json in file "filename" in SD card
    String deleteJson(const char *filename, int line);

    // Function creat new file "filename" in SD card
    std::string creatfile(std::string dirname);
    std::string creatFolder(std::string dir);

    // Function list file in SD card
    String listFile(File dir, int NUM_TABS = 1);

    String readAndDeleteLine(const char *filename, StaticJsonDocument<200> &js);

}


#endif