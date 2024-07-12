#include "NMEA_GLL.h"

// Constructor with GLL string and your GMT
NMEA_GLL::NMEA_GLL(std::string buffer, int GMT) : GLL_STRING(buffer), GMT(GMT)
{
    output.open("output.log", std::ios::out);
    // Split string
    std::string latd, longd, timed, checksum;
    std::vector<int> comma;
    for (int i = 0; i < buffer.size(); i++)
    {
        if (buffer[i] == ',')
        {
            comma.push_back(i + 1);
        }
    }

    latd = buffer.substr(comma[0], comma[1] - comma[0] - 1);
    longd = buffer.substr(comma[2], comma[3] - comma[2] - 1);
    timed = buffer.substr(comma[4], comma[5] - comma[4] - 1);
    checksum = buffer.substr(buffer.size() - 2, buffer.size());
    // cout << longd << std::endl;
    output << "LATITUDE STRING: " << latd << std::endl;
    output << "LONGITUDE STRING: " << longd << std::endl;
    output << "TIME_GPS_NEO STRING: " << timed << std::endl;
    output << "CHECKSUM STRING: " << checksum << std::endl;

    // Checksum
    char checksum_cal = 0;
    for (int i = 1; i < buffer.size() - 3; i++)
    {
        checksum_cal ^= buffer[i];
    }

    char hexChars[] = "0123456789ABCDEF";
    std::string result;
    result += hexChars[(checksum_cal >> 4) & 0xF];
    result += hexChars[checksum_cal & 0xF];

    if (result != checksum)
    {
        valid = false;
        output << "ERROR: " << "INVALID CHECKSUM" << std::endl;
        output << "CALCULATED CHECKSUM: " << result << std::endl;
        return;
    }
    else
        valid = true;

    // Decode
    output << "#######################################" << std::endl;
    // LATTITUDE
    m_lat = (latd == "") ? 0 : stod(latd) / 100;
    m_lat = int(m_lat) + ((m_lat - int(m_lat)) * 100) / 60;
    // LONGITUDE
    m_long = (longd == "") ? 0 : stod(longd) / 100;
    m_long = int(m_long) + ((m_long - int(m_long)) * 100) / 60;
    // UTC TIME_GPS_NEO
    m_time = timed.substr(0, 6);
    if (m_time != "")
    {
        int buf_time = stoi(m_time) + GMT * 10000;
        m_time = std::to_string(buf_time);
        m_time.insert(2, ":");
        m_time.insert(5, ":");
        output << "TIME_GPS_NEO: " << m_time << std::endl;
    }
    else
        output << "NONE TIME_GPS_NEO" << std::endl;

    //
    if (m_lat == 0)
    {
        output << "NONE LATITUDE" << std::endl;
    }
    else
        output << "LATITUDE: " << m_lat << std::endl;

    if (m_long == 0)
    {
        output << "NONE LONGITUDE" << std::endl;
    }
    else
        output << "LONGITUDE: " << m_long << std::endl;

    // Close file log
    output.close();
};

// Getter LATTITUDE, LONGITUDE, TIME_GPS_NEO
// Return # if error
std::string NMEA_GLL::get(NMEA_ATRIBUTE obj)
{
    // output.open("output.log", ios::app);
    if (!valid)
    {
        // output << "INVALID DATA" << std::endl;
        return "#";
    }
    switch (obj)
    {
    case LATTITUDE_GPS:
        if (m_lat == 0)
        {
            // output << "NONE LATTITUDE" << std::endl;
            return "#";
        }
        return std::to_string(m_lat);
        break;
    case LONGITUDE_GPS:
        if (m_long == 0)
        {
            // output << "NONE LONGITUDE" << std::endl;
            return "#";
        }
        return std::to_string(m_long);
        break;
    case TIME_GPS_NEO:
        if (m_time == "")
        {
            // output << "NONE TIME_GPS_NEO" << std::endl;
            return "#";
        }
        break;
    default:
        return "UNKNOWN ATTRIBUTE";
        break;
    }
}

// int main()
// {
//     NMEA_GLL nmea("$PSRF100,0,115200,8,1,0*0C", 7);
//     std::cout << nmea.get(LATTITUDE);
// }