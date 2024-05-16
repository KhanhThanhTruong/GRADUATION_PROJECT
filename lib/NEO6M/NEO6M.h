#ifndef NEO_6M_H
#define NEO_6M_H
#include <NMEA_GLL.h>
#include <Arduino.h>

class NEO6M
{
    int POSITION_NMEA;
    const char PREFIX_NMEA[10];
    char GLL[100];
    fstream output;

public:
    NEO6M();
    char *getGLL();

    const char *get(NMEA_ATRIBUTE attribute);
};

#endif