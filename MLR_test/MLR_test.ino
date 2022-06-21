// Include Library
#include <EloquentTinyML.h>
#include <eloquent_tinyml/tensorflow.h>

// Trained model from Google Colab
#include "model.h"

// 2 inputs, 1 output, 
#define N_INPUTS 2
#define N_OUTPUTS 1

// Preallocate a certain amount of memory for input, output, and intermediate arrays
#define TENSOR_ARENA_SIZE 3*1024
Eloquent::TinyML::TensorFlow::TensorFlow<N_INPUTS, N_OUTPUTS, TENSOR_ARENA_SIZE> tf;

void setup() {
    Serial.begin(115200);
    delay(4000);
    tf.begin(model);

    // check if model loaded fine
    if (!tf.isOk()) {
      Serial.print("ERROR: ");
      Serial.println(tf.getErrorMessage());

      while (true) delay(1000);
    }
}

void loop() {
        float a [] = {25.5, 29, 25.5, 28.5, 28.5};
        float b [] = {90, 64, 90, 73, 68};
        for (int i=0; i<5; i++){
        float x=a[i], y=b[i];
        float input[2] = {x,y};
        float predicted = tf.predict(input);
        Serial.print("Temperature (°C): ");
        Serial.println(x);
        Serial.print("Humidity (%): ");
        Serial.println(y);
        Serial.print("predicted wind speed: ");
        Serial.println(predicted);
        Serial.print("\n");
        }

    delay(1000);
}
