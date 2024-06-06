
#ifndef NMEA_GLL_H
#define NMEA_GLL_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;


enum NMEA_ATRIBUTE
{
    LATTITUDE,
    LONGITUDE,
    TIME
};

class NMEA_GLL
{
private:
    int GMT;
    string GLL_STRING;
    double m_lat;
    double m_long;
    string m_time;
    bool valid;
    fstream output;

public:
    // Constructor with GLL string and your GMT
    NMEA_GLL(string buffer, int GMT);

    // Getter LATTITUDE, LONGITUDE, TIME
    // Return # if error
    string get(NMEA_ATRIBUTE obj);
};

#endif