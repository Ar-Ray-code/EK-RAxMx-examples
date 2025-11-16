// Renesas RA6M5 SPI LCD Driver for ILI9341
// Adapted from ESP32-DOOM spi_lcd.c

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

extern "C" {
#include "spi_lcd.h"
}

// ILI9341 display pins for SPI0
#define TFT_CS    PIN_SPI_CS    // 89
#define TFT_DC    10
#define TFT_RST   8

// Display resolution
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

// Palette for color conversion
extern "C" int16_t lcdpal[256];

static Adafruit_ILI9341 *tft = nullptr;
static SemaphoreHandle_t dispSem = NULL;
static SemaphoreHandle_t dispDoneSem = NULL;
static uint8_t *currFbPtr = NULL;

// DMA buffers for faster transfer
#define BUFFER_SIZE 2048
static uint16_t lineBuffer[BUFFER_SIZE];

// Display task
void displayTask(void *arg) {
    Serial.println("Display task starting");

    // Initialize TFT
    tft = new Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
    tft->begin();
    tft->setRotation(1);  // Landscape mode
    tft->fillScreen(ILI9341_BLACK);

    xSemaphoreGive(dispDoneSem);

    while (1) {
        // Wait for frame data
        xSemaphoreTake(dispSem, portMAX_DELAY);

        if (currFbPtr) {
            // Set window
            tft->startWrite();
            tft->setAddrWindow(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

            // Convert 8-bit indexed color to 16-bit RGB565 and send
            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                for (int x = 0; x < SCREEN_WIDTH; x++) {
                    uint8_t pixelIndex = currFbPtr[y * SCREEN_WIDTH + x];
                    lineBuffer[x] = lcdpal[pixelIndex];
                }
                tft->writePixels(lineBuffer, SCREEN_WIDTH);
            }

            tft->endWrite();
        }

        xSemaphoreGive(dispDoneSem);
    }
}

extern "C" {

void spi_lcd_wait_finish() {
    xSemaphoreTake(dispDoneSem, portMAX_DELAY);
}

void spi_lcd_send(uint16_t *scr) {
    currFbPtr = (uint8_t*)scr;
    xSemaphoreGive(dispSem);
}

void spi_lcd_init() {
    Serial.println("spi_lcd_init()");

    // Initialize SPI
    SPI.begin();

    // Create semaphores
    dispSem = xSemaphoreCreateBinary();
    dispDoneSem = xSemaphoreCreateBinary();

    // Create display task
    xTaskCreate(displayTask, "display", 4096, NULL, 2, NULL);
}

} // extern "C"
