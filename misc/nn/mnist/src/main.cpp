/*
 * Copyright (c) Ar-Ray-code 2025
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "mnist_inference.h"
#include "mnist_test_data.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int current_image_index = 0;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    Wire.begin();

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));

        for(int i = 0; i < 10; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
        }
    } else {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println(F("MNIST Inference"));
        display.println(F("Arduino UNO R4"));
        display.println(F("Minima"));
        display.println();
        display.println(F("Ready..."));
        display.display();

        delay(2000);
    }
}

void loop() {
    digitalWrite(LED_BUILTIN, LOW);

    int expected = test_labels[current_image_index];

    unsigned long start_time = micros();

    float probabilities[10];
    mnist_predict(test_images[current_image_index], probabilities);

    unsigned long end_time = micros();
    unsigned long inference_time_us = end_time - start_time;

    int predicted = get_prediction(probabilities, 10);
    float max_prob = probabilities[predicted];
    display.clearDisplay();

    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) {
            float pixel_value = test_images[current_image_index][y * 28 + x];

            if (pixel_value > 0.5f) {
                display.fillRect(x * 2, y * 2, 2, 2, SSD1306_WHITE);
            }
        }
    }

    display.drawLine(58, 0, 58, 63, SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(62, 2);
    display.setTextSize(2);
    display.print(F("P:"));
    display.println(predicted);

    display.setCursor(62, 20);
    display.print(F("E:"));
    display.println(expected);

    display.setCursor(62, 38);
    display.setTextSize(1);
    if (predicted == expected) {
        display.println(F("OK"));
    } else {
        display.println(F("NG"));
    }

    display.setCursor(62, 48);
    display.print(max_prob * 100, 0);
    display.println(F("%"));

    display.setCursor(62, 56);
    display.print(inference_time_us / 1000);
    display.println(F("ms"));

    display.display();

    ++current_image_index;
    if (current_image_index >= NUM_TEST_IMAGES) {
        current_image_index = 0;
    }

    delay(100);
}
