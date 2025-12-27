#!/usr/bin/env python3
from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np
from tensorflow import keras


def _format_array(values: np.ndarray, values_per_line: int = 10) -> str:
    formatted = []
    for idx, value in enumerate(values):
        formatted.append(f"{value:.6f}f")
        if idx + 1 < len(values):
            formatted.append(", ")
        if (idx + 1) % values_per_line == 0:
            formatted.append("\n    ")
    return "".join(formatted)


def main() -> int:
    parser = argparse.ArgumentParser(description="Convert MNIST dense model to C header")
    parser.add_argument("--model", required=True, help="Path to .h5 Keras model")
    parser.add_argument("--out", required=True, help="Output header path")
    args = parser.parse_args()

    model_path = Path(args.model)
    out_path = Path(args.out)

    model = keras.models.load_model(model_path)
    if not model.layers:
        raise ValueError("Model has no layers")

    layer = model.layers[0]
    if not hasattr(layer, "get_weights"):
        raise ValueError("First layer has no weights")

    weights, biases = layer.get_weights()
    if weights.shape != (784, 10) or biases.shape != (10,):
        raise ValueError(
            f"Unexpected shapes: weights={weights.shape}, biases={biases.shape}"
        )

    weights_flat = weights.astype(np.float32).reshape(-1)
    biases_flat = biases.astype(np.float32).reshape(-1)

    header = """/*
 * Auto-generated from {model_name}
 */

#ifndef MNIST_MODEL_H
#define MNIST_MODEL_H

#ifdef __cplusplus
extern "C" {{
#endif

static const float mnist_model_layer_0_biases[10] = {{ {biases} }};
static const float mnist_model_layer_0_weights[7840] = {{
    {weights}
}};

#ifdef __cplusplus
}}
#endif

#endif // MNIST_MODEL_H
""".format(
        model_name=model_path.name,
        biases=_format_array(biases_flat),
        weights=_format_array(weights_flat),
    )

    out_path.write_text(header)
    print(f"Wrote {out_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
