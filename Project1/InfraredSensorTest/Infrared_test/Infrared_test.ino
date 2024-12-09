#include <Arduino.h>
#include "Infrared_generic.h"

#define INPUT_PIN 14 // Pin A0 for the sensor input
#define OUTPUT_PIN 15 // Pin A1 for generating a voltage of 2.7V

#define TEST 1

InfraredSensor sensor(INPUT_PIN);

void setup() {
  Serial.begin(9600);

  // Initialize the sensor
  sensor.begin();

  // Set OUTPUT_PIN as output and write a simulated voltage (PWM equivalent to 2.7V)
  pinMode(OUTPUT_PIN, OUTPUT);
  
  // Calculate the analogWrite value for 2.7V (assuming 3.3V reference)
  int pwmValue = (2.7 / 3.3) * 255; // 255 is max for 8-bit PWM
  //analogWrite(OUTPUT_PIN, pwmValue);
}

void loop() {
  // Perform measurement
  sensor.measure();

  // Fetch and display the measured voltage and raw value
  float voltage = sensor.getVolt();
  float rawValue = sensor.getValue();

  /*
  Serial.print("Measured Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");
  */
#if TEST
  Serial.print("Raw Sensor Value: ");
  Serial.println(rawValue);
#else
  sensor.calculateDistance();
  float d = sensor.getDistanceMeasure();
  Serial.print("Distance: "); Serial.println(d);
#endif
  // Simulate a delay between measurements
  delay(500);
}
