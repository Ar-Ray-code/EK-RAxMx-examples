#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

constexpr uint8_t LED1 = LEDR;
constexpr uint8_t LED2 = LEDG;
constexpr uint8_t LED3 = LEDB;

constexpr uint32_t LED1_PERIOD_MS = 200;
constexpr uint32_t LED2_PERIOD_MS = 300;
constexpr uint32_t LED3_PERIOD_MS = 500;

struct BlinkTaskConfig {
  const uint8_t pin;
  const TickType_t periodTicks;
};

static void LedBlinkTask(void *pvParameters) {
  auto *config = static_cast<BlinkTaskConfig *>(pvParameters);
  pinMode(config->pin, OUTPUT);
  digitalWrite(config->pin, LOW);

  TickType_t lastWakeTime = xTaskGetTickCount();
  for (;;) {
    vTaskDelayUntil(&lastWakeTime, config->periodTicks);
    digitalWrite(config->pin, !digitalRead(config->pin));
  }
}

void setup()
{
  // Tasks
  TaskHandle_t led1_task;
  TaskHandle_t led2_task;
  TaskHandle_t led3_task;

  // Configurations
  BlinkTaskConfig led1_config{LED1, pdMS_TO_TICKS(LED1_PERIOD_MS)};
  BlinkTaskConfig led2_config{LED2, pdMS_TO_TICKS(LED2_PERIOD_MS)};
  BlinkTaskConfig led3_config{LED3, pdMS_TO_TICKS(LED3_PERIOD_MS)};

  // USBSerial setup
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting scheduler ...");

  // Create tasks
  // arguments: task function, name, stack size, parameters, priority, task handle
  xTaskCreate(LedBlinkTask, "LED1", 128, &led1_config, 1, &led1_task);
  xTaskCreate(LedBlinkTask, "LED2", 128, &led2_config, 1, &led2_task);
  xTaskCreate(LedBlinkTask, "LED3", 128, &led3_config, 1, &led3_task);

  // Start the scheduler
  vTaskStartScheduler();
}

void loop()
{
    // Cannot reach here (Arduino_FreeRTOS starts the scheduler in setup())
    exit(1);
}
