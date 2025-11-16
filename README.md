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

全てのピン定義は、それぞれのボードのデータシートを参考にしてください。

### EK-RA6M5

ピン対応一覧 : [EK_RA6M5/variant.cpp](https://github.com/Ar-Ray-code/ArduinoCore-renesas-custom/blob/ex_boards/variants/EK_RA6M5/variant.cpp)

| 対応する素子 | 対応状況 | ピン名 | 実際のピン番号 |
| --- | --- | --- | --- |
| 赤LED | ✅ | `LEDR` (`LED_BUILTIN`) | 83 |
| 青LED | ✅ | `LEDB` | 15 |
| 緑LED | ✅ | `LEDG` | 82 |
| ボタン1 (S1) | - | | 16 |
| ボタン2 (S2) | - | | 17 |
| I2C SCL | ✅ | `WIRE_SCL_PIN` | 38 |
| I2C SDA | ✅ | `WIRE_SDA_PIN` | 37 |
| SPI_MOSI | `PIN_SPI_MOSI` | | 42 |
| SPI_MISO | `PIN_SPI_MISO` | | 41 |
| SPI_SCK | `PIN_SPI_SCK` | | 9 |
| SPI_CS | `PIN_SPI_CS` | | 89 |
| USB CDC | ✅ | USB CDC (USB HIGH SPEED) |
| Serial1.TX | ✅ | UART1_TX_PIN | 49 |
| Serial1.RX | ✅ | UART1_RX_PIN | 50 |
| Hrdware Ethernet | ❌ | - | - |


## 注意点

- ピン配置はArduino UNOと互換性を保つようにしていますが、同じSoCを採用するArduino Portenta C33に近づけるようにしています。そのため、Arduino UNO用のピンを整数型で指定しないでください。（Arduino UNO R3のように `D13 = 13` となっていないです。）
- `LED_BUILTIN` は `83` と紐付いています。 `D13` ではないことに注意してください。
- `Serial.begin()` は、UARTではなくUSB CDCを使用します。 `USB HIGH SPEED` のコネクタを使用してください。
- `Serial1.begin()` は、Arduino UNO互換のD1(TX)/D0(RX)を使用します。
- Ethernetには対応していません。
