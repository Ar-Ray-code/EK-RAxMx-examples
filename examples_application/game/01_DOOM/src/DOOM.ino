// DOOM for Renesas EK-RA6M5
// Based on ESP32-DOOM port
// https://github.com/espressif/esp32-doom

#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

extern "C" {
#include "doomtype.h"
#include "d_main.h"
#include "spi_lcd.h"

extern void jsInit(void);
extern int doom_main(int argc, const char * const *argv);

void doomEngineTask(void *pvParameters) {
    Serial.println("DOOM Engine Task Starting...");

    const char *argv[] = {"doom", "-cout", "ICWEFDA", NULL};
    doom_main(3, argv);

    // Should never reach here
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

} // extern "C"

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("====================================");
    Serial.println("   DOOM for Renesas EK-RA6M5");
    Serial.println("   Based on ESP32-DOOM");
    Serial.println("====================================");
    Serial.println();

    // Initialize display
    Serial.println("Initializing display...");
    spi_lcd_init();

    // Initialize input
    Serial.println("Initializing input...");
    jsInit();

    // Create DOOM engine task
    Serial.println("Creating DOOM engine task...");
    xTaskCreate(
        doomEngineTask,
        "doomEngine",
        16384,  // Stack size (reduced from ESP32's 22480 due to RAM constraints)
        NULL,
        3,      // Priority
        NULL
    );

    Serial.println("Setup complete. DOOM should start soon...");
}

void loop() {
    // Empty - everything runs in FreeRTOS tasks
    vTaskDelay(pdMS_TO_TICKS(1000));
}
