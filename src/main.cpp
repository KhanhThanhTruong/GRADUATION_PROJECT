#include <Arduino.h>
#include <PMS7003.h>

PMS7003 pms7003(2, 3);

void setup()
{
  pms7003.init();
  // delay(500);
  pms7003.setMode(ACTIVE_MODE);
  // delay(100);
}

void loop()
{
  uint8_t *data = pms7003.readData();
  Serial.println(pms7003.getData(PM1_0));
  delay(5000);
}
