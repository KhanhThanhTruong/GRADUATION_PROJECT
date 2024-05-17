#include <Arduino.h>
#include <PMS7003.h>

PMS7003 pms7003(GPIO_NUM_22, GPIO_NUM_23);

void setup()
{
    Serial.begin(9600);
    pms7003.init();
    // delay(500);
    pms7003.setMode(PASSIVE_MODE);
    // delay(100);

  // Serial.begin(9600);
  // Serial2.begin(9600, SERIAL_8N1, GPIO_NUM_22, GPIO_NUM_23);
}

void loop()
{
  // char c;
  // if(Serial2.available())
  // {
  //   c=Serial2.read();
  //   Serial.print(c);
  // }
  
  // delay(300);
  uint8_t *data = pms7003.readBytes();
  if(data == nullptr)
  {
    Serial.println("No data");
    delay(3000);
    return;
  }
  for(int i=0;i<FRAME_LENGTH;i++)
  {
    Serial.printf("BYTE %d:",i);
    Serial.println(data[i],HEX);
  }
  Serial.println();
  pms7003.decodeData();
  delay(3000);
  // if(data == nullptr)
  // {
  //   Serial.println("No data");
  //   // pms7003.setMode(ACTIVE_MODE);
  //   return;
  // }

  // switch (pms7003.decodeData())   
  // {

  // case START_CHAR_ERROR:
  //   Serial.println("Start char error");
  //   break;
  // case CHECKSUM_ERROR:
  //   Serial.println("Checksum error");
  //   break;
  // default:
  //   Serial.println("No error");
  //   break;
  // }
  // Serial.println(pms7003.getData(PM1_0));
  // delay(5000);
}
