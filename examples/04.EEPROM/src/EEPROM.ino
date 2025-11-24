// button : P005

#include <Arduino.h>
#include <EEPROM.h>

const int buttonPin = 17;  // the number of the pushbutton pin
int addr = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

  while (digitalRead(buttonPin) != LOW)
  {
    unsigned long currentValue = 0;

    int lowByte = EEPROM.read(0);
    int highByte = EEPROM.read(1);
    int duration = lowByte | (highByte << 8);
    currentValue += duration;

    Serial.print("Current Value: ");
    Serial.print(currentValue);
    Serial.println(" (waiting for LOW)");
    delay(500);
  }

  Serial.println("");
  Serial.println("Button LOW detected, start measuring...");

  unsigned long start = millis();
  while (digitalRead(buttonPin) == LOW)
    ;
  unsigned long duration = millis() - start;
  Serial.println("OK!");

  EEPROM.write(addr, duration & 0xFF);
  EEPROM.write(addr + 1, (duration >> 8) & 0xFF);
  addr += 2;

  Serial.print("Duration recorded: ");
  Serial.print(duration);
  Serial.println(" ms");

  delay(100);
}

void loop() {
  for (int i = 0; i < addr; i += 2) {
    int lowByte = EEPROM.read(i);
    int highByte = EEPROM.read(i + 1);
    int duration = lowByte | (highByte << 8);
    Serial.print("Duration at addr ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(duration);
  }

  delay(1000);
}