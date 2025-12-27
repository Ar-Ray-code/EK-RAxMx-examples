#!/usr/bin/env python3
from tensorflow import keras
import numpy as np

# Load MNIST dataset
(train_images, train_labels), (test_images, test_labels) = keras.datasets.mnist.load_data()

# Reshape to (N, 784) and normalize to 0-1 range
train_images = train_images.reshape(-1, 784) / 255.0
test_images = test_images.reshape(-1, 784) / 255.0

# Create model without Flatten layer (direct 784 input)
model = keras.Sequential([
    keras.layers.Dense(10, activation='softmax', input_shape=(784,))
])

model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

print("Model architecture:")
print(model.summary())

# Train model
print("\nTraining model...")
model.fit(train_images, train_labels, epochs=10, batch_size=128, verbose=1)

# Evaluate
print("\nEvaluating on test set...")
test_loss, test_acc = model.evaluate(test_images, test_labels, verbose=0)
print(f'Test accuracy: {test_acc:.4f}')

# Save the model
model.save('dense_model.h5')
print("Model saved to dense_model.h5")
