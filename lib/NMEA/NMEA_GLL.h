
#ifndef NMEA_GLL_H
#define NMEA_GLL_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
// using namespace std;


enum NMEA_ATRIBUTE
{
    LATTITUDE_GPS,
    LONGITUDE_GPS,
    TIME_GPS_NEO
};

class NMEA_GLL
{
private:
    int GMT;
    std::string GLL_STRING;
    double m_lat;
    double m_long;
    std::string m_time;
    bool valid;
    std::fstream output;

public:
    // Constructor with GLL string and your GMT
    NMEA_GLL(std::string buffer, int GMT);

    // Getter LATTITUDE, LONGITUDE, TIME_GPS_NEO
    // Return # if error
    std::string get(NMEA_ATRIBUTE obj);
};

#endif