# MNIST 学習・変換ツール

`misc/nn/mnist/train` に、MNIST の学習と C ヘッダー変換を行うツールを Docker で動かせる形で用意しています。
本プロジェクトの推論コード (`misc/nn/mnist/src/mnist_inference.cpp`) に合わせて、**784 → 10 の単層 Dense** モデルを出力します。

## 必要なもの
- Docker
- Docker Compose

## 手順
### 1. Docker イメージのビルド  
```bash
cd /home/h6x-arai/Documents/firmware_poc/EK-RAxMx-examples/misc/nn/mnist/train
docker compose build
```

### 2. 学習の実行
```bash
docker compose run --rm mnist python train_dense_model.py
```
- MNIST データセットがコンテナ内にダウンロードされます。
- 学習済みモデルは `misc/nn/mnist/train/dense_model.h5` として保存されます。

### 3. ヘッダーへの変換
```bash
docker compose run --rm mnist \
  python convert_to_header.py --model dense_model.h5 --out ../src/mnist_model.h
```
- `misc/nn/mnist/src/mnist_model.h` を上書きします。
- 推論コードは `mnist_model_layer_0_weights[j * 10 + i]` の並びを想定しているため、変換スクリプトは同じ並び順で出力します。

## 出力物
- 学習済みモデル: `misc/nn/mnist/train/dense_model.h5`
- 生成ヘッダー: `misc/nn/mnist/src/mnist_model.h`
