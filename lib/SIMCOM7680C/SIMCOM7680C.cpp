#include <SIMCOM7680C.h>

// Triển khai các phương thức của lớp A7680C

void A7680C::begin(int baudarate)
{
    a7680c_uart.begin(baudarate);
}
String A7680C::getResponse()
{   
    delay(100);
    String response;
    unsigned long startTime = millis(); // Bắt đầu theo dõi thời gian
    while (millis() - startTime < RESPONSE_TIMEOUT_A7680C)
    {
        if (a7680c_uart.available())
        {
            char c = a7680c_uart.read();
            response += c;
        }

        if (response.length() == 0)
        {
            return "NO RESPONSE";
        }
        else
        {
            return response;
        }
    }
    // delay(100);
    // return "NO RESPONSE";
}

bool A7680C::sendATCommand(String command)
{
    // Serial.print("MCU: ");
    // Serial.println(command);
    // if (!a7680c_uart.availableForWrite())
    // {
    //     return false;
    // }

    a7680c_uart.println(command);
    // a7680c_uart.flush();
    delay(10);

    return true;
}
bool A7680C::clear_rx_tx_buffer()
{
}

bool A7680C::callNumber(String number)
{
    sendATCommand("ATE0");
    // getResponse();
    String command = "ATD" + number + ";";
    sendATCommand(command);
    // return checkResponse(getResponse());
    return true;
}

bool A7680C::sendSMS(String number, String message)
{   
    sendATCommand("ATE0");
    // delay(100);
    getResponse();
    sendATCommand("AT+CMGF?");
    sendATCommand("AT+CMGF=1");
    getResponse();


    sendATCommand("AT+CSCS=\"GSM\"");
    getResponse();
    String command = "AT+CMGS=\"" + number + "\"";
    sendATCommand(command);
    getResponse();
    sendATCommand(message);
    a7680c_uart.write(26); // Gửi ký tự Ctrl+Z để kết thúc tin nhắn
    return checkResponse(getResponse());
}

bool A7680C::getIMEI()
{
    return sendATCommand("AT+GSN");
}

bool A7680C::checkResponse(String response)
{
    return (response.indexOf("ERROR") == -1) ? true : false;
}

bool A7680C::mqtt_client_idconfg(String client_id)
{
    // AT+CMQTTACCQ=0,”client_id”,0
    String command = "AT+CMQTTACCQ=0,\"" + client_id + "\",0";
    sendATCommand(command);
    return checkResponse(getResponse());
}

bool A7680C::mqtt_connect(String id, int time_alive, int QoS, String server_link, int port, String user, String pass)
{
    sendATCommand("ATE0");
    String command0 = "+CMQTTCONNECT:0,\"" + server_link + ":" + String(port) + "\"," + String(time_alive) + "," + String(QoS) + ",\"" + user + "\",\"" + pass + "\"";
    // String command1 = "+CMQTTCONNECT:1,\"" + server_link + ":" + String(port) + "\"," + String(time_alive) + "," + String(QoS) + ",\"" + user + "\",\"" + pass + "\"";
    sendATCommand("AT+CMQTTCONNECT?");
    // getResponse();
    if (getResponse().indexOf("demo.thingsboard.io") != -1)
    {
        return true;
    }
    else
    {
        sendATCommand("AT+CMQTTSTART");
        getResponse();
        mqtt_client_idconfg(id);
        getResponse();
        String command = "AT+CMQTTCONNECT=0,\"" + server_link + ":" + String(port) + "\"," + String(time_alive) + "," + String(QoS) + ",\"" + user + "\",\"" + pass + "\"";
        sendATCommand(command);
        return checkResponse(getResponse());
    }
}

bool A7680C::reset()
{
    sendATCommand("AT+CRESET");
    if (checkResponse(getResponse()))
    {
        delay(15000);
        return true;
    }
    // delay(15000);
    return 0;
}

bool A7680C::mqtt_publish(String topic, String message)
{
    sendATCommand("AT+CMQTTTOPIC=0," + String(topic.length()));
    getResponse();
    sendATCommand(topic);
    getResponse();
    sendATCommand("AT+CMQTTPAYLOAD=0," + String(message.length()));
    getResponse();
    sendATCommand(message);
    getResponse();
    sendATCommand("AT+CMQTTPUB=0,1,60,0");
    return checkResponse(getResponse());
}
