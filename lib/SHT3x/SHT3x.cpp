#include "SHT3x.h"

SHT3x::SHT3x(uint8_t sht_address) : _sht_address{sht_address}
{
    _mode = MODE_SHT_NONE;
    // memset(data, 0, 6);
    setalltozero();
    // Wire.begin();
}

bool SHT3x::init()
{
    Wire.begin();
    return true;
}
void SHT3x::setalltozero()
{
    memset(_data, 0, SHT3x_DATA_LENGTH);
    // _mode = MODE_SHT_NONE;
    _temperature = 0;
    _humidity = 0;
    temperature_16bit = 0;
    humidity_16bit = 0;
}
bool SHT3x::setMode(MODE_SHT3x_NAME mode)
{
    switch (mode)
    {
    case MODE_SHT_SINGLESHOT:
        // if (getMode() == mode)
        // {
        //     return true;
        // }
        // Wire.beginTransmission(_sht_address);
        // // Repeatability High and Clock stretching enable
        // Wire.write(0x2C);
        // Wire.write(0x06);
        // if (Wire.endTransmission() == 0)
        // {
        //     _mode = mode;
        //     return true;
        // }
        // else
        //     return false;
        _mode=mode;
        return true;
        break;
    case MODE_SHT_PERIODIC:
        if (getMode() == mode)
        {
            return true;
        }
        Wire.beginTransmission(_sht_address);
        // Repeatability High and mps = 1
        Wire.write(0x21);
        Wire.write(0x30);
        if (Wire.endTransmission() == 0)
        {
            _mode = mode;
            return true;
        }
        else
            return false;
        break;
    default:
        _mode = MODE_SHT_NONE;
        return false;
        break;
    }
}

MODE_SHT3x_NAME SHT3x::getMode()
{
    return _mode;
}

ERROR_SHT3x_READ SHT3x::readBytes()
{
    switch (getMode())
    {
    case MODE_SHT_PERIODIC:
        Wire.beginTransmission(_sht_address);
        Wire.write(0xE0);
        Wire.write(0x00);
        if (Wire.endTransmission(false) != 0)
        {
            setalltozero();
            return COMMUNITCATION_FAILED;
            break;
        }
        // delay(10);
        if (Wire.requestFrom(_sht_address, (uint8_t)SHT3x_DATA_LENGTH) != SHT3x_DATA_LENGTH)
        {
            // Serial.println("Failed to read from SHT3x");
            setalltozero();
            return DATA_READ_FAILED;
            break;
        }

        memset(_data, 0, 6);
        Wire.readBytes(_data, 6);
        return NO_ERROR;
        break;
    case MODE_SHT_SINGLESHOT:
        Wire.beginTransmission(_sht_address);
        Wire.write(0x2c);
        Wire.write(0x06);
        if (Wire.endTransmission(false) != 0)
        {
            setalltozero();
            return COMMUNITCATION_FAILED;
            break;
        }

        if (Wire.requestFrom(_sht_address, (uint8_t)6) != 6)
        {
            // Serial.println("Failed to read from SHT3x");
            setalltozero();
            return DATA_READ_FAILED;
        }

        // memset(rx_bytes, 0, 6);
        Wire.readBytes(_data, 6);
        return NO_ERROR;
        break;

    default:
        return MODE_ERROR;
        break;
    }
}

uint8_t SHT3x::getChecksum(uint8_t *data, size_t size)
{
    uint8_t crc = SHT3x_CRC_INITIAL_VALUE;
    for (int i = 0; i < size; i++)
    {
        crc ^= data[i];
        for (int bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ SHT3x_CRC_POLYNOMIAL;
            }
            else
            {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}
ERROR_SHT3x_READ SHT3x::decode()
{
    // Check data is not zero
    if (_data[0] == 0 && _data[1] == 0 && _data[2] == 0 && _data[3] == 0 && _data[4] == 0 && _data[5] == 0)
    {
        return DATA_ZERO;
    }

    // Get checksum
    uint8_t data_tem[2] = {_data[0], _data[1]};
    uint8_t data_hum[2] = {_data[3], _data[4]};
    uint8_t checksum_temp = getChecksum(data_tem, 2);
    uint8_t checksum_hum = getChecksum(data_hum, 2);
    if (checksum_temp != _data[2] || checksum_hum != _data[5])
    {
        return CHECKSUM_ERROR;
    }

    // Decode byte to temperature and humidity
    uint16_t t_ticks = ((uint16_t)_data[0] << 8) | _data[1];
    uint16_t rh_ticks = ((uint16_t)_data[3] << 8) | _data[4];
    _temperature = -45.0f + (175.0f * (t_ticks / 65535.0f));
    _humidity = 100 * (rh_ticks / 65535.0f);
    if (_humidity > 100)
    {
        _humidity = 100;
    }
    if (_humidity < 0)
    {
        _humidity = 0;
    }
    return NO_ERROR;
}

float SHT3x::get(DATA_SHT3x_TARGET target)
{
    switch (target)
    {
    case TEMPERATURE_SHT:
        return _temperature;
        break;
    case HUMIDITY_SHT:
        return _humidity;
        break;

    default:
        return 0;
        break;
    }
}

uint8_t *SHT3x::getdata()
{
    return _data;
}