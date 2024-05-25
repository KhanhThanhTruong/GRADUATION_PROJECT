#ifndef SHT3x_H
#define SHT3x_H

#include <Arduino.h>
#include <Wire.h>
#define SHT3x_I2C_ADDRESS_DEFAULT 0x44
#define SHT3x_I2C_ADDRESS_ALT     0x45
#define CMD_SHT3x_LENGTH          2
#define SHT3x_DATA_LENGTH         6
#define SHT3x_CRC_INITIAL_VALUE   0xFF
#define SHT3x_CRC_POLYNOMIAL      0x31
const uint8_t CMD_SHT3x_SINGLESHOT[3][2] = {{0x2C, 0x06}, {0x2C, 0x0D},{0x2C, 0x10}};
const uint8_t CMD_SHT3x_PERIODIC[15][2]={{0X20,0X32},{0X20,0x24},{0x20,0x2F},{0x21,0x30},{0x21,0x26},{0x21,0x2D},{0x22,0x36},{0x22,0x20},{0x22,0x2B},{0x23,0x34},{0x23,0x22},{0x23,0x29},{0x27,0x37},{0x27,0x21},{0x27,0x2A}};

enum REPEATABILITY_SHT3x
{
    REPEATABILITY_SHT3x_HIGH=0,
    REPEATABILITY_SHT3x_MEDIUM=1,
    REPEATABILITY_SHT3x_LOW=2
};

enum CLOCK_STRENGTH_SHT3x
{
    SHT3X_ENABLED,
    SHT3X_DISABLED,
    SHT3x_NONE
};

enum MPS_SHT3x
{
    SHT3X_MPS_0_5=0,
    SHT3X_MPS_1_0=1,
    SHT3X_MPS_2_5=2,
    SHT3X_MPS_4_0=3,
    SHT3X_MPS_10_0=4,
    SHT3x_MPS_NONE
};
enum DATA_SHT3x_TARGET
{
    TEMPERATURE_SHT ,
    HUMIDITY_SHT
    
};
enum MODE_SHT3x_NAME
{
    MODE_SHT_SINGLESHOT,
    MODE_SHT_PERIODIC, 
    MODE_SHT_NONE
};
enum ERROR_SHT3x_READ
{
    COMMUNITCATION_FAILED,
    DATA_READ_FAILED,
    MODE_ERROR,
    NO_ERROR,
    DATA_ZERO,
    CHECKSUM_ERROR
};

class SHT3x
{
    uint16_t temperature_16bit, humidity_16bit;
    uint8_t _data[SHT3x_DATA_LENGTH];
    float _temperature, _humidity;
    uint8_t _sht_address;
    MODE_SHT3x_NAME _mode;
    public:
    SHT3x(uint8_t sht_address);
    bool init();
    //Function set mode of sht3x
    //Return true if success
    //Return false if fail
    bool setMode(MODE_SHT3x_NAME mode_type);
    //Get current mode
    MODE_SHT3x_NAME getMode();
    //Read 6 bytes from SHT3x and storage to data
    ERROR_SHT3x_READ readBytes();
    //Decode 6 bytes from SHT3x to temperature and humidity
    //Return NO_ERROR if success
    ERROR_SHT3x_READ decode();
    //Get checksum
    uint8_t getChecksum(uint8_t* data,size_t length);
    //Get temperature and humidity
    float get(DATA_SHT3x_TARGET target);
    //Set all data to zero
    void setalltozero();

    uint8_t *getdata();


};

#endif