# DOOM-Style Raycaster for EK-RA6M5

このプロジェクトは、Renesas EK-RA6M5評価ボード向けのDOOMスタイルのレイキャスティングエンジンです。

## 概要

DOOM風の3Dレイキャスティングエンジンを実装しています。ILI9341 TFTディスプレイ（320x240、SPI0接続）を使用して、リアルタイムで3D空間をレンダリングします。

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

## 機能

- リアルタイムレイキャスティング
- 16x16のマップデータ
- 距離に基づいた壁の陰影処理
- 自動ナビゲーション（デモモード）
- 衝突検出

## ビルド方法

```bash
cd examples_application/game/01_DOOM
pio run
```

## アップロード方法

```bash
pio run --target upload
```

## カスタマイズ

### マップの変更

`worldMap` 配列を編集することで、独自のマップを作成できます：
- `1` = 壁
- `0` = 空き空間

### プレイヤー制御の追加

現在はデモモードで自動移動していますが、ボタン入力を追加することで手動制御が可能です：

```cpp
// ボタンピンを定義
#define BTN_FORWARD 16
#define BTN_BACKWARD 17

// setup()内で初期化
pinMode(BTN_FORWARD, INPUT_PULLUP);
pinMode(BTN_BACKWARD, INPUT_PULLUP);

// loop()内で制御
if (digitalRead(BTN_FORWARD) == LOW) {
  float newX = playerX + cos(playerAngle) * moveSpeed;
  float newY = playerY + sin(playerAngle) * moveSpeed;
  if (worldMap[(int)newY][(int)newX] == 0) {
    playerX = newX;
    playerY = newY;
  }
}
```

## パフォーマンス

- フレームレート: 約20 FPS
- レイ解像度: 320本（画面幅に対応）
- 最大描画距離: 16ユニット

## 注意事項

- このコードは簡略化されたレイキャスティングエンジンです。完全なDOOMの移植ではありません。
- メモリ制限のため、テクスチャマッピングは実装していません。
- パフォーマンス最適化の余地があります。

## ライセンス

このプロジェクトは、元のリポジトリのライセンスに従います。
