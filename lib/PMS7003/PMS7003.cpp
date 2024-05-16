#include "PMS7003.h"

PMS7003::PMS7003(int8_t rx, int8_t tx)
    : _rx(rx), _tx(tx)
{
    // Set data to zero
    for (int i = 0; i < FRAME_LENGTH; i++)
    {
        data[i] = 0x00;
    }
    // Set checksum to zero
    checksum = 0x00;
    // Set mode to 0
    mode = 0;
    // Set pm to zero
    pm1_0 = 0;
    pm2_5 = 0;
    pm10 = 0;
};

void PMS7003::setMode(MODE_NAME mode_type)
{
    if (Serial.availableForWrite())
    {
        switch (mode_type)
        {
        case ACTIVE_MODE:
            if (getMode() != ACTIVE_MODE)
            {
                Serial.write(ACTIVE, CMD_LENGTH);
                mode = (1 << ACTIVE_BIT);
            }

            Serial.println("PMS7003 IS ACTIVE");
            break;
        case PASSIVE_MODE:
            if (getMode() != PASSIVE_MODE)
            {
                Serial.write(PASSIVE, CMD_LENGTH);
                mode = (1 << PASSIVE_BIT);
            }
            Serial.println("PMS7003 IS PASSIVE");
            break;
        case SLEEP_MODE:
            if (getMode() != SLEEP_MODE)
            {
                Serial.write(SLEEP, CMD_LENGTH);
                mode = mode | (1 << SLEEP_BIT);
            }
            Serial.println("PMS7003 IS SLEEP");
            break;
        case WAKE_MODE:
            if (getMode() == SLEEP_MODE)
            {
                Serial.write(WAKE, CMD_LENGTH);
                mode = mode & ~(1 << SLEEP_BIT);
            }

            Serial.println("PMS7003 IS WAKE");
            break;
        default:
            Serial.println("UNKNOWN MODE");
            break;
        }
    }
    delay(TIME_WAIT_SET_MODE);
}

void PMS7003::init()
{
    Serial.begin(UART_BAUD);
    Serial.println("PMS7003 init complete");
    delay(TIME_WAIT_START);
}

MODE_NAME PMS7003::getMode()
{
    if (mode == (1 << ACTIVE_BIT))
        return ACTIVE_MODE;
    if (mode == (1 << PASSIVE_BIT))
        return PASSIVE_MODE;
    if ((mode & (1 << SLEEP_BIT)) == (1 << SLEEP_BIT))
        return SLEEP_MODE;

    return UNKOWN_MODE;
}
uint8_t *PMS7003::readData()
{
    uint32_t start_time = millis();
    while (true)
    {
        if (Serial.available() >= FRAME_LENGTH)
        {
            Serial.readBytes(data, FRAME_LENGTH);
            decodeData();
            return data;
        }
        if (millis() - start_time > TIMEOUT)
        {
            Serial.println("PMS7003 TIMEOUT");
            return nullptr;
        }
    }
}

void PMS7003::decodeData()
{   
    //Check 2 bytes start
    if(data[0]!= START_CHAR1 || data[1]!= START_CHAR2)
    {
        Serial.println("PMS7003 START CHAR ERROR");
        return;
    }
    //Check checksum
    uint16_t sum = 0;
    for (int i = 0; i < FRAME_LENGTH - 2; i++) {
        sum += data[i];
    }
    uint16_t checksum = (data[FRAME_LENGTH - 2] << 8) | data[FRAME_LENGTH - 1];
    if(sum != checksum)
    {
        Serial.println("PMS7003 CHECKSUM ERROR");
        return;
    }

    pm1_0 = (data[4] << 8) | data[5];
    pm2_5 = (data[6] << 8) | data[7];
    pm10 = (data[8] << 8) | data[9];

    pm1_0_atm = (data[10] << 8) | data[11];
    pm2_5_atm = (data[12] << 8) | data[13];
    pm10_atm = (data[14] << 8) | data[15];
}

uint16_t PMS7003::getData(DATA_TARGET target)
{
    switch (target)
    {
    case PM1_0:
        return pm1_0;
    case PM2_5:
        return pm2_5;
    case PM10:
        return pm10;
    case PM1_0_ATM:
        return pm1_0_atm;
    case PM2_5_ATM:
        return pm2_5_atm;
    case PM10_ATM:
        return pm10_atm;
    default:
        return 0;
    }
}