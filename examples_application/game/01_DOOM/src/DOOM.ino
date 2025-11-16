#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <math.h>

// ILI9341 display pins for SPI0
#define TFT_CS    PIN_SPI_CS    // 89
#define TFT_DC    10
#define TFT_RST   8
#define TFT_MOSI  PIN_SPI_MOSI  // 42
#define TFT_MISO  PIN_SPI_MISO  // 41
#define TFT_CLK   PIN_SPI_SCK   // 9

// Display resolution
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

// Game settings
#define MAP_WIDTH  16
#define MAP_HEIGHT 16
#define FOV        60.0
#define MAX_DEPTH  16.0

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// World map (1 = wall, 0 = empty)
const uint8_t worldMap[MAP_HEIGHT][MAP_WIDTH] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,1,1,1,0,0,0,0,1,1,1,0,0,1},
  {1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
  {1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
  {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
  {1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
  {1,0,0,1,1,1,0,0,0,0,1,1,1,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Player position and direction
float playerX = 8.0;
float playerY = 8.0;
float playerAngle = 0.0;

// Movement speed
const float moveSpeed = 0.05;
const float rotSpeed = 0.03;

// Frame buffer for rendering optimization
uint16_t frameBuffer[SCREEN_WIDTH];

void setup() {
  Serial.begin(115200);

  // Initialize display
  tft.begin();
  tft.setRotation(1);  // Landscape mode
  tft.fillScreen(ILI9341_BLACK);

  // Show startup message
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(80, 100);
  tft.println("DOOM-Style");
  tft.setCursor(70, 130);
  tft.println("Ray Caster");
  delay(2000);

  Serial.println("DOOM-style raycaster started!");
  Serial.println("Controls: Auto-rotate demo");
}

void loop() {
  // Auto-rotate for demo (remove this and add button controls for real gameplay)
  playerAngle += rotSpeed;
  if (playerAngle > 2 * PI) playerAngle -= 2 * PI;

  // Auto-move forward
  float newX = playerX + cos(playerAngle) * moveSpeed;
  float newY = playerY + sin(playerAngle) * moveSpeed;

  // Collision detection
  if (worldMap[(int)newY][(int)newX] == 0) {
    playerX = newX;
    playerY = newY;
  } else {
    // If hit wall, turn around
    playerAngle += PI / 2;
  }

  // Render the scene
  renderScene();

  // Small delay for visibility
  delay(50);
}

void renderScene() {
  // Cast rays for each vertical screen column
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    // Calculate ray angle
    float rayAngle = (playerAngle - FOV / 2 * PI / 180.0) + (x * FOV * PI / 180.0 / SCREEN_WIDTH);

    // Ray direction
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    // Current position
    float rayX = playerX;
    float rayY = playerY;

    // Distance to wall
    float distance = 0.0;
    bool hitWall = false;

    // Cast ray until we hit a wall or max depth
    while (!hitWall && distance < MAX_DEPTH) {
      distance += 0.1;

      int testX = (int)(rayX + rayDirX * distance);
      int testY = (int)(rayY + rayDirY * distance);

      // Check if ray is out of bounds
      if (testX < 0 || testX >= MAP_WIDTH || testY < 0 || testY >= MAP_HEIGHT) {
        hitWall = true;
        distance = MAX_DEPTH;
      } else if (worldMap[testY][testX] == 1) {
        hitWall = true;
      }
    }

    // Calculate wall height based on distance
    int ceiling = (int)((SCREEN_HEIGHT / 2.0) - SCREEN_HEIGHT / distance);
    int floor = SCREEN_HEIGHT - ceiling;

    // Calculate wall color based on distance (darker = farther)
    uint8_t shade = (uint8_t)(255 * (1.0 - (distance / MAX_DEPTH)));
    if (shade < 20) shade = 20;

    // Create color (darker red for walls)
    uint16_t wallColor = tft.color565(shade, shade / 2, shade / 4);

    // Store in frame buffer
    frameBuffer[x] = wallColor;

    // Draw column
    // Sky
    tft.drawFastVLine(x, 0, ceiling, ILI9341_DARKGREY);

    // Wall
    if (floor > ceiling) {
      tft.drawFastVLine(x, ceiling, floor - ceiling, wallColor);
    }

    // Floor
    tft.drawFastVLine(x, floor, SCREEN_HEIGHT - floor, ILI9341_BROWN);
  }
}
