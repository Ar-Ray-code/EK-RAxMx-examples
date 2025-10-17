#include <Arduino.h>
#include <Arduino_CAN.h>

static uint32_t const CAN_ID = 0x20;
static uint32_t msg_cnt = 0;

void setup()
{
  Serial.begin(115200);

  if (!CAN.begin(CanBitRate::BR_500k))
  {
    Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }
  Serial.println("CAN.begin(...) Successful.");
}


void loop()
{
  uint8_t const msg_data[] = {0xCA,0xFE,0,0,0,0,0,0};
  memcpy((void *)(msg_data + 3), &msg_cnt, sizeof(msg_cnt));
  CanMsg const msg(CanStandardId(CAN_ID), sizeof(msg_data), msg_data);

  if (int const rc = CAN.write(msg); rc < 0)
  {
    Serial.print  ("CAN.write(...) failed with error code ");
    Serial.println(rc);
  }

  msg_cnt++;
  delay(1000);
}