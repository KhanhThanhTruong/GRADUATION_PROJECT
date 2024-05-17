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

SETMODE_NOTIFI PMS7003::setMode(MODE_NAME mode_type)
{
    if (Serial2.availableForWrite())
    {
        switch (mode_type)
        {
        case ACTIVE_MODE:
            if (getMode() != ACTIVE_MODE)
            {
                if (Serial2.availableForWrite())
                {
                    Serial2.write(ACTIVE, CMD_LENGTH);
                    mode = (1 << ACTIVE_BIT);
                }
                else
                    return SET_MODE_FAIL;
            }

            // Serial.println("PMS7003 IS ACTIVE");
            return SET_MODE_SUCCESS;
            break;
        case PASSIVE_MODE:
            if (getMode() != PASSIVE_MODE)
            {
                if (Serial2.availableForWrite())
                {
                    Serial2.write(PASSIVE, CMD_LENGTH);
                    mode = (1 << PASSIVE_BIT);
                }
                else
                    return SET_MODE_FAIL;
            }
            // Serial.println("PMS7003 IS PASSIVE");
            return SET_MODE_SUCCESS;
            break;
        case SLEEP_MODE:
            if (getMode() != SLEEP_MODE)
            {
                if (Serial2.availableForWrite())
                {
                    Serial2.write(PASSIVE, CMD_LENGTH);
                    mode = (1 << PASSIVE_BIT);
                }
                else
                    return SET_MODE_FAIL;
            }
            // Serial.println("PMS7003 IS SLEEP");
            return SET_MODE_SUCCESS;
            break;
        case WAKE_MODE:
            if (getMode() == SLEEP_MODE)
            {
                if (Serial2.availableForWrite())
                {
                    Serial2.write(WAKE, CMD_LENGTH);
                    mode = mode & ~(1 << SLEEP_BIT);
                }
                else
                    return SET_MODE_FAIL;
            }

            // Serial.println("PMS7003 IS WAKE");
            return SET_MODE_SUCCESS;
            break;
        default:
            // Serial.println("UNKNOWN MODE");
            return SET_MODE_FAIL;
            break;
        }
    }
    delay(TIME_WAIT_SET_MODE);
}

void PMS7003::init()
{
    Serial2.begin(UART_BAUD_PMS7003, SERIAL_8N1, _rx, _tx);
    // Serial.begin(UART_BAUD_PMS7003);
    if (Serial.availableForWrite())
    {
        Serial.println("PMS7003 init complete");
    }
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

void PMS7003::setalldatatozero()
{
    for (int i = 0; i < FRAME_LENGTH; i++)
    {
        data[i] = 0x00;
    }
    pm1_0 = 0;
    pm2_5 = 0;
    pm10 = 0;
    pm1_0_atm = 0;
    pm2_5_atm = 0;
    pm10_atm = 0;
}
uint8_t *PMS7003::readBytes()
{

    uint32_t start_time = millis();
    while (true)
    {
        if (Serial2.available())
        {
            if (Serial2.read() == START_CHAR1 && Serial2.read() == START_CHAR2)
            {
                data[0] = START_CHAR1;
                data[1] = START_CHAR2;
                if (Serial2.available() >= FRAME_LENGTH - 2)
                    for (int i = 2; i < FRAME_LENGTH; i++)
                    {
                        data[i] = Serial2.read();
                    }
                return data;
            }
        }
        if (millis() - start_time > TIMEOUT)
        {
            // Serial.println("PMS7003 TIMEOUT");
            setalldatatozero();
            return nullptr;
        }
    }
}

uint8_t *PMS7003::readBytesBaseOnMode()
{
    switch (getMode())
    {
    case ACTIVE_MODE:
        return readBytes();
        break;
    case PASSIVE_MODE:
        if (Serial2.availableForWrite())
        {
            Serial2.write(PASSIVE_REQUEST_DATA, CMD_LENGTH);
            delay(TIME_WAIT_PASSIVE_REQUEST_DATA);
        }
        return readBytes();
    default:
        return nullptr;
        break;
    }
}

ERROR_DECODE_PMS7003 PMS7003::decodeData()
{

    // Check 2 bytes start
    if (data[0] != START_CHAR1 || data[1] != START_CHAR2)
    {
        // if (Serial.availableForWrite())
        // Serial.println("PMS7003 START CHAR ERROR");
        setalldatatozero();
        return START_CHAR_ERROR;
        // return;
    }
    // Check checksum
    uint16_t sum = 0;
    for (int i = 0; i < FRAME_LENGTH - 2; i++)
    {
        sum += data[i];
    }
    uint16_t checksum = (data[FRAME_LENGTH - 2] << 8) | data[FRAME_LENGTH - 1];
    Serial.print("CHECK SUM: ");
    Serial.println(checksum);
    Serial.print("CHECKSUM CALC: ");
    Serial.println(sum);
    if (sum != checksum)
    {
        // if (Serial.availableForWrite())
        // Serial.println("PMS7003 CHECKSUM ERROR");
        setalldatatozero();
        return CHECKSUM_ERROR;
    }

    pm1_0 = (data[4] << 8) | data[5];
    pm2_5 = (data[6] << 8) | data[7];
    pm10 = (data[8] << 8) | data[9];

    pm1_0_atm = (data[10] << 8) | data[11];
    pm2_5_atm = (data[12] << 8) | data[13];
    pm10_atm = (data[14] << 8) | data[15];

    return NO_ERROR;
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