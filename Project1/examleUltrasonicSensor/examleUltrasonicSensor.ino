#include "UltrasonicSensor.h"

// Define ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 2;

// Create an instance of UltrasonicSensor
UltrasonicSensor sensor(trigPin, echoPin);

void setup() {
    Serial.begin(9600);
    sensor.begin();
}

void loop() {
    // Retrieve and display the distance
    float distance = sensor.getDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(100);
}
