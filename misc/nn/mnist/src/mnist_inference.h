/*
 * Copyright (c) Ar-Ray-code 2025
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MNIST_INFERENCE_H
#define MNIST_INFERENCE_H

#ifdef __cplusplus
extern "C" {
#endif

void mnist_predict(const float*, float*);
void softmax(float *, const int);

int get_prediction(const float *, const int);

#ifdef __cplusplus
}
#endif

#endif // MNIST_INFERENCE_H
