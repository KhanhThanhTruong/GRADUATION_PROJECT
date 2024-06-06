/*
   PMS7003 Operating Modes:

   - Passive Mode: In this mode, the sensor does not actively send data. It only responds to commands from the host microcontroller
                    to request data. This mode consumes less power compared to active mode. It is suitable for applications where
                    periodic data collection is required.

   - Active Mode: In this mode, the sensor actively sends data at regular intervals without the need for explicit requests from
                  the host microcontroller. It continuously measures and transmits data at predefined intervals. This mode consumes
                  more power but provides real-time data updates. It is suitable for applications requiring continuous monitoring
                  of air quality.

   - Sleep Mode: This mode is a low-power state in which the sensor is in an idle state, conserving power. It can be used to
                 minimize power consumption when the sensor is not actively used. The sensor can be quickly awakened from sleep
                 mode to resume normal operation.
*/

#ifndef PMS7003_H
#define PMS7003_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define CMD_LENGTH 7
#define FRAME_LENGTH 32
#define ACTIVE_BIT 7
#define PASSIVE_BIT 6
#define SLEEP_BIT 5
#define UART_BAUD_PMS7003 9600
#define TIMEOUT 100
#define START_CHAR1 0x42
#define START_CHAR2 0X4D
#define FRAME_LENGTH_HIGH_BYTE 2
#define FRAME_LENGTH_LOW_BYTE 3
#define PM1_0_HIGH_BYTE 4
#define PM1_0_LOW_BYTE 5
#define PM2_5_HIGH_BYTE 6
#define PM2_5_LOW_BYTE 7
#define PM10_HIGH_BYTE 8
#define PM10_LOW_BYTE 9
#define TIME_WAIT_START 1000
#define TIME_WAIT_SET_MODE 100
#define TIME_WAIT_PASSIVE_REQUEST_DATA 100
enum MODE_NAME
{
    ACTIVE_MODE,
    PASSIVE_MODE,
    SLEEP_MODE,
    WAKE_MODE,
    UNKOWN_MODE
};
enum ERROR_DECODE_PMS7003
{
    NO_ERROR_PMS,
    // FRAME_ERROR,
    CHECKSUM_ERROR_PMS,
    // TIMEOUT_ERROR,
    // NO_DATA_ERROR,
    START_CHAR_ERROR
};

enum SETMODE_NOTIFI
{
    SET_MODE_SUCCESS,
    SET_MODE_FAIL
};

enum DATA_TARGET
{
    PM1_0_PMS,
    PM2_5_PMS,
    PM10_PMS,
    PM1_0_ATM,
    PM2_5_ATM,
    PM10_ATM
};

// Active Mode command code
const uint8_t ACTIVE[CMD_LENGTH] = {0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71};
// Passive Mode command code
const uint8_t PASSIVE[CMD_LENGTH] = {0x42, 0x4D, 0xE1, 0x00, 0x00, 0x01, 0x70};
// Request data in Passive Mode command code
const uint8_t PASSIVE_REQUEST_DATA[CMD_LENGTH] = {0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71};
// Sleep Mode command code
const uint8_t SLEEP[CMD_LENGTH] = {0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x73};
// Wake PMS7003 from Sleep Mode command code
const uint8_t WAKE[CMD_LENGTH] = {0x42, 0x4D, 0xE4, 0x00, 0x01, 0x01, 0x74};

class PMS7003
{
private:
    SoftwareSerial _pms_serial;
    uint8_t mode;
    int8_t _rx, _tx;
    uint8_t data[FRAME_LENGTH];
    uint8_t checksum;
    uint16_t pm1_0, pm2_5, pm10, pm1_0_atm, pm2_5_atm, pm10_atm;
    
    // void sendMSG_to_user(char* msg);
public:
    //Constructor _rx,__tx
    PMS7003(int8_t RX, int8_t TX);
    //Function init PMS7003
    //Function set Baudrate, RX, TX of PMS7003
    void init();
    //Function set mode of PMS7003
    //Return SET_MODE_SUCCESS if success
    //Return SET_MODE_FAIL if fail
    SETMODE_NOTIFI setMode(MODE_NAME mode);
    //Function read 32 bytes from PMS7003 start with start char 1 and 2
    //Return nullptr if timeout and not enough data
    uint8_t *readBytes();
    //Function decode 32 bytes from PMS7003
    //Return ERROR_DECODE_PMS7003 if error
    //Return NO_ERROR if no error 
    ERROR_DECODE_PMS7003 decodeData();
    //Function get data from PMS7003
    //Return 0 if data not ready
    uint16_t getData(DATA_TARGET data);
    //Function get mode from PMS7003
    MODE_NAME getMode();
    //Function set 32 bytes previous to zero
    //Function set all pm to zero
    void setalldatatozero();

    //Function read data base on mode
    //Return data
    //Return nullptr if timeout or mode is sleep
    uint8_t* readBytesBaseOnMode();
};

#endif