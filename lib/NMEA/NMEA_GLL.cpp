#include "NMEA_GLL.h"

// Constructor with GLL string and your GMT
NMEA_GLL::NMEA_GLL(string buffer, int GMT) : GLL_STRING(buffer), GMT(GMT)
{
    output.open("output.log", ios::out);
    // Split string
    string latd, longd, timed, checksum;
    vector<int> comma;
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
    // cout << longd << endl;
    output << "LATITUDE STRING: " << latd << endl;
    output << "LONGITUDE STRING: " << longd << endl;
    output << "TIME STRING: " << timed << endl;
    output << "CHECKSUM STRING: " << checksum << endl;

    // Checksum
    char checksum_cal = 0;
    for (int i = 1; i < buffer.size() - 3; i++)
    {
        checksum_cal ^= buffer[i];
    }

    char hexChars[] = "0123456789ABCDEF";
    string result;
    result += hexChars[(checksum_cal >> 4) & 0xF];
    result += hexChars[checksum_cal & 0xF];

    if (result != checksum)
    {
        valid = false;
        output << "ERROR: " << "INVALID CHECKSUM" << endl;
        return;
    }
    else
        valid = true;

    // Decode
    output << "#######################################" << endl;
    // LATTITUDE
    m_lat = (latd == "") ? 0 : stod(latd) / 100;
    m_lat = int(m_lat) + ((m_lat - int(m_lat)) * 100) / 60;
    // LONGITUDE
    m_long = (longd == "") ? 0 : stod(longd) / 100;
    m_long = int(m_long) + ((m_long - int(m_long)) * 100) / 60;
    // UTC TIME
    m_time = timed.substr(0, 6);
    if (m_time != "")
    {
        int buf_time = stoi(m_time) + GMT * 10000;
        m_time = to_string(buf_time);
        m_time.insert(2, ":");
        m_time.insert(5, ":");
        output << "TIME: " << m_time << endl;
    }
    else
        output << "NONE TIME" << endl;

    //
    if (m_lat == 0)
    {
        output << "NONE LATITUDE" << endl;
    }
    else
        output << "LATITUDE: " << m_lat << endl;

    if (m_long == 0)
    {
        output << "NONE LONGITUDE" << endl;
    }
    else
        output << "LONGITUDE: " << m_long << endl;

    // Close file log
    output.close();
};

// Getter LATTITUDE, LONGITUDE, TIME
// Return # if error
string NMEA_GLL::get(NMEA_ATRIBUTE obj)
{
    // output.open("output.log", ios::app);
    if (!valid)
    {
        // output << "INVALID DATA" << endl;
        return "#";
    }
    switch (obj)
    {
    case LATTITUDE:
        if (m_lat == 0)
        {
            // output << "NONE LATTITUDE" << endl;
            return "#";
        }
        return to_string(m_lat);
        break;
    case LONGITUDE:
        if (m_long == 0)
        {
            // output << "NONE LONGITUDE" << endl;
            return "#";
        }
        return to_string(m_long);
        break;
    case TIME:
        if (m_time == "")
        {
            // output << "NONE TIME" << endl;
            return "#";
        }
        return m_time;
        break;
    default:
        return "UNKNOWN ATTRIBUTE";
        break;
    }

}