#ifndef SIMCOM7680C_H
#define SIMCOM7680C_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define BAUDARATE_A7680C 115200
#define RESPONSE_TIMEOUT_A7680C 50
#define COMMAND_TIMEOUT_A7680C 50
#define a7680c_uart Serial2

// String mqtt_buff[12]={"AT","ATE0","AT+CGSN","ATI","AT+CPIN?","AT+CIMI","AT+CICCID","AT+CSQ","AT+CMQTTDISC=0,60","AT+CMQTTREL=0","AT+CMQTTSTOP","AT+CMQTTSTART"};
class A7680C
{

public:
    // A7680C(uint8_t rx, uint8_t tx);
    void begin(int baud=BAUDARATE_A7680C);
    bool sendATCommand(String command);
    bool callNumber(String number);
    bool sendSMS(String number, String message);
    bool getIMEI();
    String getResponse();
    bool reset();
    bool mqtt_connect(String id,int time_alive, int QoS, String server_link,int port, String user, String pass);
    bool checkResponse(String response);
    bool mqtt_publish(String topic, String message);
    bool mqtt_subscribe(String topic, int QoS);
    bool mqtt_client_idconfg(String id);
};

#endif