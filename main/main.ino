#include <Arduino_LSM9DS1.h>
#include "functions.h"  // Include the header file with the function declarations
#include <string.h>  // Include string.h for strcmp

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  float x, y, z;
  
  // Read acceleration
  readAcceleration(x, y, z);
  
  // Calculate the roll angle
  float roll = calculateRoll(x, y, z);

  // Display the roll angle
  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.println(" degrees");

  delay(1000);  // Delay of 1 second
}
