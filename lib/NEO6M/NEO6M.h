#ifndef NEO_6M_H
#define NEO_6M_H
#include <NMEA_GLL.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#define NEO6M_BAUD 9600
class NEO6M
{   
    uint8_t _rx, _tx;
    int POSITION_NMEA;
    const char PREFIX_NMEA[10];
    char GLL[100];
    fstream output;
    SoftwareSerial _gps;
public:
    NEO6M(uint8_t rx, uint8_t tx);
    //Function init Serial communication
    void init();
    //Function get GPGLL string from NEO6M by Serial without PREFIX
    //Example ",2059.90773,N,10550.87083,E,035157.00,A,A*6D"
    char *getGLL();
    //Function get attribute: LATTITUDE, LONGITUDE, TIME
    const char *get(NMEA_ATRIBUTE attribute);

};

#endif