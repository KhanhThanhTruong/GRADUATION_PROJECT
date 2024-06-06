#include "NEO6M.h"

NEO6M::NEO6M(uint8_t rx, uint8_t tx)
    : POSITION_NMEA{0}, PREFIX_NMEA{"$GPGLL"}, GLL{""}, _rx{rx}, _tx{tx} {};

void NEO6M::init()
{
    // SoftwareSerial _gps(_rx, _tx);
    _gps.begin(NEO6M_BAUD, SWSERIAL_8N1, _rx, _tx);
}
char *NEO6M::getGLL()
{
    POSITION_NMEA = 0;
    // unsigned long startTime = millis(); // Start tracking time
    unsigned long timeout = 2000;        // 100ms timeout

    while (true)
    {
        // Check if the elapsed time has reached the timeout
        // if (millis() - startTime >= timeout)
        {
            // Exit the loop if 100ms have passed
            // break;
        }

        if (_gps.available())
        {
            char c = _gps.read();

            // Reset the start time when data is received
            // startTime = millis();

            // Check if the current character matches the expected character in the prefix
            if (c == PREFIX_NMEA[POSITION_NMEA])
            {
                POSITION_NMEA++;

                // If the entire prefix has been matched
                if (POSITION_NMEA == strlen(PREFIX_NMEA))
                {
                    // Read the rest of the sentence until the newline character
                    size_t bytesRead = _gps.readBytesUntil('\n', GLL, sizeof(GLL) - 1);
                    GLL[bytesRead] = '\0'; // Null-terminate the string

                    // Reset the position for reading the next sentence
                    POSITION_NMEA = 0;

                    // Return the complete NMEA sentence
                    return GLL;
                }
            }
            else
            {
                // If characters don't match, reset the position
                POSITION_NMEA = 0;
            }
        }
    }

    // char GLL_error_str[]="#";
    // If the loop exits due to timeout, return a special value or handle appropriately
    return nullptr; // Or another value appropriate for your context
}

const char *NEO6M::get(NMEA_ATRIBUTE attribute)
{
    string GLL_buf = string(getGLL());
    if (GLL_buf == string(nullptr))
    {
        return "#";
    }
    GLL_buf = GLL_buf.substr(0, GLL_buf.size() - 1);
    NMEA_GLL nmea("$GPGLL" + GLL_buf, 7);
    return nmea.get(attribute).c_str();
}