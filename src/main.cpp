#include <Wire.h>
#include <Arduino.h>
#include <SHT3x.h>

SHT3x sht(SHT3x_I2C_ADDRESS_DEFAULT);
// To use a specific sensor instead of probing the bus use this command:
// SHTSensor sht(SHTSensor::SHT3X);

void setup()
{
  // put your setup code here, to run once:

  // Wire.begin();
  Serial.begin(9600);
  sht.init();

  while (sht.setMode(MODE_SHT_SINGLESHOT) == false)
  {
    Serial.println("SET MODE FAIL");
    delay(1000);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:

  switch (sht.readBytes())
  {
  case COMMUNITCATION_FAILED:
    Serial.println("COMMUNICATION FAILED");
    delay(1000);
    return;
    // break;
  case DATA_READ_FAILED:
    Serial.println("DATA READ FAILED");
    delay(1000);
    return;
  default:
    Serial.println("DATA READ OK");
    break;
  }

  // uint8_t *data = sht.getdata();

  // for (int i = 0; i < 6; i++)
  // {
  //   Serial.print(data[i], HEX);
  //   Serial.print(" ");
  // }

  // Serial.println();

  switch (sht.decode())
  {
  case DATA_ZERO:
    Serial.println("DATA ZERO");
    delay(1000);
    return;
    break;
  case CHECKSUM_ERROR:
    Serial.println("CHECKSUM ERROR");
    delay(1000);
    return;
    break;
  default:
    break;
  }

  Serial.print("Temperature: ");
  Serial.println(sht.get(TEMPERATURE));
  Serial.print("Humidity: ");
  Serial.println(sht.get(HUMIDITY));

  delay(5000);
}
