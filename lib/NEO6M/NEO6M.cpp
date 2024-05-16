#include "NEO6M.h"

NEO6M::NEO6M() : POSITION_NMEA{0}, PREFIX_NMEA{"$GPGLL"}, GLL{""} {};
char *NEO6M::getGLL()
{
    // output.open("output.log", ios::out);
    POSITION_NMEA = 0;
    // strcpy(PREFIX_NMEA, "$GPGLL");
    while (true)
    {
        if (Serial.available())
        {
            char c = Serial.read();
            if (c == PREFIX_NMEA[POSITION_NMEA])
            {
                POSITION_NMEA++;
                if (POSITION_NMEA == strlen(PREFIX_NMEA))
                {
                    size_t bytesRead = Serial.readBytesUntil('\n', GLL, sizeof(GLL) - 1);
                    GLL[bytesRead] = '\0'; // Kết thúc chuỗi bằng ký tự null
                    // Serial.println(GLL);
                    // POSITION_NMEA = 0; // Đặt lại vị trí để đọc chuỗi tiếp theo
                    return GLL;
                }
            }
            else
            {
                POSITION_NMEA = 0;
            }
        }
    }
}

const char *NEO6M::get(NMEA_ATRIBUTE attribute)
{
    string GLL_buf = string(getGLL());
    NMEA_GLL nmea("$GPGLL" + GLL_buf, 7);
    return nmea.get(attribute).c_str();
}