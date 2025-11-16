// Renesas RA6M5 Gamepad Implementation
// Adapted from ESP32-DOOM gamepad.c
// Uses board buttons for input

#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

extern "C" {
#include "gamepad.h"
#include "doomdef.h"
#include "doomtype.h"
#include "d_event.h"

// Button pins (from EK-RA6M5 pinout)
#define BTN_1 16  // S1
#define BTN_2 17  // S2

// Keyboard emulation variables
int usejoystick = 0;
int joyleft, joyright, joyup, joydown;

// Current button state
static volatile uint16_t buttonState = 0;

// Button mapping for EK-RA6M5
// Using simple 2-button mapping for demo
typedef struct {
    uint16_t mask;
    int *key;
} ButtonMap;

static const ButtonMap buttonMap[] = {
    {0x01, &key_fire},          // BTN_1 = Fire
    {0x02, &key_use},           // BTN_2 = Use/Open
    {0, NULL},
};

void gamepadPoll(void) {
    static uint16_t oldButtonState = 0xFFFF;
    uint16_t newButtonState = buttonState;
    event_t ev;

    for (int i = 0; buttonMap[i].key != NULL; i++) {
        if ((oldButtonState ^ newButtonState) & buttonMap[i].mask) {
            ev.type = (newButtonState & buttonMap[i].mask) ? ev_keyup : ev_keydown;
            ev.data1 = *buttonMap[i].key;
            D_PostEvent(&ev);
        }
    }

    oldButtonState = newButtonState;
}

void buttonTask(void *arg) {
    Serial.println("Button task starting");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(20));

        // Read button states (active low)
        buttonState = 0;
        if (digitalRead(BTN_1) == LOW) buttonState |= 0x01;
        if (digitalRead(BTN_2) == LOW) buttonState |= 0x02;
    }
}

void gamepadInit(void) {
    Serial.println("gamepadInit: Initializing gamepad");

    // Initialize button pins
    pinMode(BTN_1, INPUT_PULLUP);
    pinMode(BTN_2, INPUT_PULLUP);
}

void jsInit() {
    gamepadInit();

    // Create button polling task
    xTaskCreate(buttonTask, "buttons", 2048, NULL, 1, NULL);
}

} // extern "C"
