#include "NEO6M.h"

NEO6M::NEO6M(uint8_t rx, uint8_t tx)
    : POSITION_NMEA{0}, PREFIX_NMEA{"$GNGLL"}, GNSS_STR{""}, _rx{rx}, _tx{tx} {};

void NEO6M::init()
{
    // SoftwareSerial _gps(_rx, _tx);
    _gps.begin(NEO6M_BAUD, SWSERIAL_8N1, _rx, _tx);
}

char *NEO6M::getGLL2()
{
    POSITION_NMEA = 0;
    unsigned long startTime = millis(); // Start tracking time
    unsigned long timeout = 2000;       // 100ms timeout

    while (!_gps.available())
    {
        // Check if the elapsed time has reached the timeout
        if (millis() - startTime >= 100)
        {
            // Exit the loop if 100ms have passed
            GNSS_STR[0] = '#';
            GNSS_STR[1] = '\0';
            return GNSS_STR;
        }
    }

    while (true)
    {
        // Check if the elapsed time has reached the timeout
        if (millis() - startTime >= timeout)
        {
            // Exit the loop if 100ms have passed
            break;
        }

        if (_gps.available())
        {
            char c = _gps.read();

            // Reset the start time when data is received
            startTime = millis();

            // Check if the current character matches the expected character in the prefix
            if (c == PREFIX_NMEA[POSITION_NMEA])
            {
                POSITION_NMEA++;

                // If the entire prefix has been matched
                if (POSITION_NMEA == strlen(PREFIX_NMEA))
                {
                    // Read the rest of the sentence until the newline character
                    size_t bytesRead = _gps.readBytesUntil('\n', GNSS_STR, sizeof(GNSS_STR) - 1);
                    GNSS_STR[bytesRead] = '\0'; // Null-terminate the string

                    // Reset the position for reading the next sentence
                    POSITION_NMEA = 0;

                    // Return the complete NMEA sentence
                    return GNSS_STR;
                }
            }
            else
            {
                // If characters don't match, reset the position
                POSITION_NMEA = 0;
            }
        }
    }

    // If the loop exits due to timeout, return a special value or handle appropriately
    GNSS_STR[0] = '#';
    GNSS_STR[1] = '\0';
    return "#"; // Or another value appropriate for your context
}

char *NEO6M::getGLL()
{
    POSITION_NMEA = 0;
    unsigned long startTime = millis(); // Start tracking time
    unsigned long timeout = 2000;       // 100ms timeout

    while (true)
    {
        // Check if the elapsed time has reached the timeout
        if (millis() - startTime >= 50)
        {
            // Exit the loop if 100ms have passed
            GNSS_STR[0] = '#';
            GNSS_STR[1] = '\0';
            return GNSS_STR;
        }

        if (_gps.available())
        {
            char c = _gps.read();

            // Reset the start time when data is received
            startTime = millis();
            while (true)
            {
                if (c == '$')
                {
                    _gps.readBytesUntil('\n', GNSS_STR, sizeof(GNSS_STR) - 1);

                    for (int i = 0; i < sizeof(NEO_GNSS_PREFIX) / sizeof(NEO_GNSS_PREFIX[0]); i++)
                    {
                        if (strstr(GNSS_STR, NEO_GNSS_PREFIX[i]) != nullptr)
                        {
                            char completeNMEA[100];
                            snprintf(completeNMEA, 100, "%s%s", NEO_GNSS_PREFIX[i], GNSS_STR);
                            return completeNMEA; // Trả về chuỗi NMEA hoàn chỉnh
                        }
                    }
                }
                if (millis() - startTime >= timeout)
                {
                    GNSS_STR[0] = '#';
                    GNSS_STR[1] = '\0';
                    return GNSS_STR;
                }
            }
        }
    }
}

const char *NEO6M::get(NMEA_ATRIBUTE attribute)
{
    std::string GLL_buf = std::string(GNSS_STR);
    if (GLL_buf == "#")
    {
        return "#";
    }
    GLL_buf = GLL_buf.substr(0, GLL_buf.size() - 1);
    NMEA_GLL nmea("$GPGLL" + GLL_buf, 7);
    return nmea.get(attribute).c_str();
}
char *NEO6M::getGNSS()
{
    POSITION_NMEA = 0;
    unsigned long startTime = millis(); // Start tracking time
    unsigned long timeout = 2000;       // 100ms timeout

    while (!_gps.available())
    {
        // Check if the elapsed time has reached the timeout
        if (millis() - startTime >= 50)
        {
            // Exit the loop if 100ms have passed
            GNSS_STR[0] = '#';
            GNSS_STR[1] = '\0';
            return GNSS_STR;
        }
    }

    while (true)
    {
        // Check if the elapsed time has reached the timeout
        if (millis() - startTime >= timeout)
        {
            // Exit the loop if 100ms have passed
            break;
        }

        if (_gps.available())
        {
            _gps.readBytesUntil('\n', GNSS_STR, sizeof(GNSS_STR) - 1);
            // sprintf(GNSS_STR, "%s", str.c_str());
            for(int i = 0; i < sizeof(NEO_GNSS_PREFIX) / sizeof(NEO_GNSS_PREFIX[0]); i++)
            {
                if(strstr(GNSS_STR, NEO_GNSS_PREFIX[i]) != nullptr)
                {   
                    PREFIX_TYPE = i;
                    return strstr(GNSS_STR, NEO_GNSS_PREFIX[i]);
                }
            }
            // return GNSS_STR;
        }
    }

    // If the loop exits due to timeout, return a special value or handle appropriately
    GNSS_STR[0] = '#';
    GNSS_STR[1] = '\0';
    return "#"; // Or another value appropriate for your context
}

