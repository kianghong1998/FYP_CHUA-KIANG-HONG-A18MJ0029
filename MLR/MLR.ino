// Include Library
#include <EloquentTinyML.h>
#include <eloquent_tinyml/tensorflow.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

// Trained model from Google Colab
#include "model.h"

// 2 inputs, 1 output, 
#define N_INPUTS 2
#define N_OUTPUTS 1

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Preallocate a certain amount of memory for input, output, and intermediate arrays
#define TENSOR_ARENA_SIZE 3*1024
Eloquent::TinyML::TensorFlow::TensorFlow<N_INPUTS, N_OUTPUTS, TENSOR_ARENA_SIZE> tf;

LiquidCrystal_I2C lcd (0x27, 20, 4);

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

    Serial.begin(9600);
  
    // Initialize the LCD connected 
    lcd. init ();
   
    // Turn on the backlight on LCD. 
    lcd. backlight ();
    dht.begin();
    
}

void loop() {
  lcd. setCursor (0, 0);
  lcd.print("MLR model");
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float input[2] = {t,h};
  float predicted = tf.predict(input);
  Serial.print("Temperature (°C): ");
  Serial.println(t);
  Serial.print("Humidity (%): ");
  Serial.println(h);
  Serial.print("predicted wind speed: ");
  Serial.println(predicted);
  Serial.print("\n");

  lcd. setCursor (0, 1);
  lcd.print("Temperature: ");
  lcd.print(t);
  lcd.print("C ");
  
  lcd. setCursor (0, 2);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");

  lcd. setCursor (0, 3);
  lcd.print("Wind Speed: ");
  lcd.print(predicted);
  lcd.print("km/h");


}
