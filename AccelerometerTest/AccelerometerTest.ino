#include <Wire.h>
#include "i2c_generic.h"
#include "Accelerometer_generic.h"

i2c i2cObj(Wire); // Create i2c object with default Wire instance
Accelerometer accel(i2cObj); // Pass i2c object to Accelerometer

void setup() {
    Serial.begin(9600);
    while (!Serial);

    if (!accel.begin()) {
        Serial.println("Failed to initialize accelerometer!");
        while (1);
    }

    Serial.println("Accelerometer initialized.");
}

void loop() {
    if (accel.accelerationAvailable()) {
        float angle = 0;
        angle = accel.getAngle(angle);

        Serial.print("Roll angle: ");
        Serial.println(angle);
    }
    delay(500);
}
