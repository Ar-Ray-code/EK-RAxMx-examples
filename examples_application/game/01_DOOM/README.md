# DOOM for Renesas EK-RA6M5

このプロジェクトは、ESP32-DOOM（https://github.com/espressif/esp32-doom）をベースにした、Renesas EK-RA6M5評価ボード向けの本格的なDOOM移植です。

## 概要

PrBoomエンジンをベースにした完全なDOOMの実装です。ILI9341 TFTディスプレイ（320x240、SPI0接続）を使用し、FreeRTOSでマルチタスク処理を実現しています。

## ハードウェア要件

- EK-RA6M5 評価ボード
- ILI9341 TFTディスプレイ（320x240ピクセル）
- SPI0接続

## ピン配置

| 機能 | ピン番号 | 定義名 |
|------|----------|--------|
| SPI MOSI | 42 | PIN_SPI_MOSI |
| SPI MISO | 41 | PIN_SPI_MISO |
| SPI CLK | 9 | PIN_SPI_SCK |
| SPI CS | 89 | PIN_SPI_CS |
| TFT DC | 10 | TFT_DC |
| TFT RST | 8 | TFT_RST |

## 主な機能

- 完全なPrBoomゲームエンジン
- 320x240解像度でのリアルタイムレンダリング
- FreeRTOSベースのマルチタスク処理
- ボタン入力対応（2ボタン）
- 最適化されたSPI DMA転送

## 制限事項

**注意**: このプロジェクトは現在作業中です。以下の制限があります：

1. **WADファイル**: DOOM1.WADファイルのサポートが未実装です。WADファイルをプログラムに埋め込むか、SDカードから読み込む機能が必要です。
2. **メモリ**: RA6M5のSRAM（512KB）はESP32のPSRAMより少ないため、メモリ使用量の最適化が必要です。
3. **サウンド**: メモリ制約のため、サウンドは無効化されています。
4. **ネットワーク**: マルチプレイヤー機能は無効化されています。

## ビルド方法

### 前提条件

1. PlatformIOのインストール
2. 十分なメモリ（ビルドには大量のRAMが必要）

### ビルド

```bash
cd examples_application/game/01_DOOM
pio run
```

**警告**: PrBoomエンジンは大規模なため、ビルドには時間がかかり、大量のメモリを消費します。

## WADファイルの準備

**重要**: DOOMを実行するには、WADファイルが必要です。

### オプション1: shareware版DOOM1.WADを使用

shareware版のDOOM1.WADファイルを入手してください（無料で合法的に配布されています）。

### オプション2: プログラムに埋め込む（未実装）

将来的には、WADファイルをプログラムメモリに埋め込む機能を追加予定です。

### オプション3: SDカードから読み込む（推奨・未実装）

SDカードサポートを追加し、そこからWADファイルを読み込む方法が最も現実的です。

## 現在の状態

このプロジェクトは**開発中**です。現時点では：

- ✅ PrBoomエンジンのコアコードを移植
- ✅ ILI9341ディスプレイドライバを実装
- ✅ FreeRTOSタスクを実装
- ✅ 基本的な入力システムを実装
- ❌ WADファイルローダーが未完成
- ❌ メモリ最適化が未完成
- ❌ 実際の動作確認が未完成

## 次のステップ

1. WADファイルのロード機能を実装（SDカード対応）
2. メモリ使用量を最適化
3. ビルドエラーを修正
4. 実機でのテストと調整

## 技術的な詳細

### アーキテクチャ

- **ゲームエンジン**: PrBoom 2.5.1.5
- **ディスプレイ**: ILI9341 (320x240, 16-bit color)
- **RTOS**: FreeRTOS（Arduino統合版）
- **メモリ管理**: 簡略化されたmmap実装

### ファイル構成

```
01_DOOM/
├── prboom/                 # PrBoomエンジン（ESP32-DOOMから）
│   ├── include/           # ヘッダーファイル
│   └── src/               # ソースファイル
├── src/
│   ├── compat/            # RA6M5適応レイヤー
│   │   ├── config.h      # ビルド設定
│   │   ├── spi_lcd_ra.cpp    # ディスプレイドライバ
│   │   ├── i_system_ra.c     # システムインターフェース
│   │   ├── i_sound_ra.c      # サウンドスタブ
│   │   ├── i_network_ra.c    # ネットワークスタブ
│   │   └── gamepad_ra.cpp    # 入力処理
│   └── DOOM.ino           # メインエントリーポイント
├── platformio.ini         # ビルド設定
└── README.md              # このファイル
```

## ライセンス

- **PrBoom**: GNU General Public License v2
- **ESP32-DOOM**: Apache License 2.0
- **このポート**: GPLv2（PrBoomのライセンスに従う）

## クレジット

- オリジナルDOOM: id Software
- PrBoom: PrBoomチーム
- ESP32-DOOM: Espressif Systems
- RA6M5ポート: このリポジトリ

## 参考リンク

- [ESP32-DOOM](https://github.com/espressif/esp32-doom)
- [PrBoom](http://prboom.sourceforge.net/)
- [EK-RA6M5](https://www.renesas.com/ja/products/microcontrollers-microprocessors/ra-cortex-m-mcus/ek-ra6m5-evaluation-kit-ra6m5-mcu-group)
