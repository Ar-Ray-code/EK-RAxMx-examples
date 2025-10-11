# EK-RAxMx-examples
ルネサス製RAマイコン評価キット用Arduinoサンプルスケッチ（platformio）

<img width="539" height="306" alt="image" src="https://github.com/user-attachments/assets/cf4286a6-384c-4ba8-9aa6-35c21cb38e33" />


## 依存環境

### ソフトウェア

- PlatformIO
- [Ar-Ray-code/platform-renesas-ra-custom](https://github.com/Ar-Ray-code/platform-renesas-ra-custom)
- [Ar-Ray-code/ArduinoCore-renesas-custom](https://github.com/Ar-Ray-code/ArduinoCore-renesas-custom)

### ハードウェア

- [EK-RA6M5](https://www.renesas.com/ja/design-resources/boards-kits/ek-ra6m5)
  

## ブートローダー

[Ar-Ray-code/arduino-renesas-bootloader](https://github.com/Ar-Ray-code/arduino-renesas-bootloader)のREADMEに初期化手順を載せています。

## 特定のピン

### EK-RA6M5

| 対応する素子 | 対応状況 | ピン名 | 実際のピン番号 |
| --- | --- | --- | --- |
| 赤LED | ✅ | `LEDR` (`LED_BUILTIN`) | 83 |
| 青LED | ✅ | `LEDB` | 15 |
| 緑LED | ✅ | `LEDG` | 82 |
| ボタン1 (S1) | - | | 16 |
| ボタン2 (S2) | - | | 17 |
| I2C SCL | ✅ | `WIRE_SCL_PIN` | 38 |
| I2C SDA | ✅ | `WIRE_SDA_PIN` | 37 |
| SPI_MOSI | - | | 42 |
| SPI_MISO | - | | 41 |
| SPI_SCK | - | | 9 |
| SPI_CS | - | | 89 |
| USB CDC | ✅ | USB CDC (USB HIGH SPEED) |
| Serial1.TX | ✅ | 49 |
| Serial1.RX | ✅ | 50 |


## 注意点

- ピン配置はArduino UNOと互換性を保つようにしていますが、同じSoCを採用するArduino Portenta C33に近づけるようにしています。そのため、Arduino UNO用のピンを整数型で指定しないでください。（Arduino UNO R3のように `D13 = 13` となっていないです。）
- `LED_BUILTIN` は `83` と紐付いています。 `D13` ではないことに注意してください。
- `Serial.begin()` は、UARTではなくUSB CDCを使用します。 `USB HIGH SPEED` のコネクタを使用してください。
- `Serial1.begin()` は、Arduino UNO互換のD1(TX)/D0(RX)を使用します。
