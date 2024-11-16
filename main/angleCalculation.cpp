#include "functions.h"
#include <Arduino_LSM9DS1.h>
#include <math.h>

// Function to read acceleration data
void readAcceleration(float& x, float& y, float& z) {
  if (IMU.accelerationAvailable()) {  // Check if acceleration data is available
    IMU.readAcceleration(x, y, z);  // Read acceleration data
  }
}

// Function to calculate the roll angle (roll)
float calculateRoll(float x, float y, float z) {
  return atan2(y, sqrt(x * x + z * z)) * 180 / PI;  // Convert from radians to degrees
}
