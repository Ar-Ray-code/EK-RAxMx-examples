/*
 * Copyright (c) Ar-Ray-code 2025
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mnist_inference.h"
#include "mnist_model.h"
#include <math.h>

void mnist_predict(const float* input, float* output) {
    for (int i = 0; i < 10; i++) {
        float sum = mnist_model_layer_0_biases[i];
        for (int j = 0; j < 784; j++) {
            sum += input[j] * mnist_model_layer_0_weights[j * 10 + i];
        }
        output[i] = sum;
    }
    softmax(output, 10);
}

void softmax(float* values, const int length) {
    float max_val = values[0];
    for (int i = 1; i < length; i++) {
        if (values[i] > max_val) {
            max_val = values[i];
        }
    }

    float sum_exp = 0.0f;
    for (int i = 0; i < length; i++) {
        values[i] = expf(values[i] - max_val);
        sum_exp += values[i];
    }

    for (int i = 0; i < length; i++) {
        values[i] /= sum_exp;
    }
}

int get_prediction(const float* probabilities, const int length) {
    int predicted = 0;
    float max_prob = probabilities[0];

    for (int i = 1; i < length; i++) {
        if (probabilities[i] > max_prob) {
            max_prob = probabilities[i];
            predicted = i;
        }
    }

    return predicted;
}
