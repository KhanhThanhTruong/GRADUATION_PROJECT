#ifndef NEO_6M_H
#define NEO_6M_H
#include <NMEA_GLL.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#define NEO6M_BAUD 9600
const char NEO_GNSS_PREFIX[3][10] = {"$GNGLL","$GNRMC","$GNGGA"};
class NEO6M
{   
    uint8_t _rx, _tx;
    int POSITION_NMEA;
    const char PREFIX_NMEA[10];
    size_t PREFIX_TYPE;
    char GNSS_STR[100];
    std::fstream output;
    SoftwareSerial _gps;
    float _lat, _long;
    String _time;
public:
    NEO6M(uint8_t rx, uint8_t tx);
    //Function init Serial communication
    void init();
    //Function get GPGLL string from NEO6M by Serial Ver2
    char *getGLL2();
    //Function get GPGLL string from NEO6M by Serial without PREFIX
    //Example ",2059.90773,N,10550.87083,E,035157.00,A,A*6D"
    char *getGLL();
    //Function get attribute: LATTITUDE, LONGITUDE, TIME
    const char *get(NMEA_ATRIBUTE attribute);
    //Decode GNSS String
    void decodeGNSS();
    //Function get GNSS String
    char *getGNSS();


};

#endif