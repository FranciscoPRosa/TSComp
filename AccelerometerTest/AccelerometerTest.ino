#include <Arduino_LSM9DS1.h>

float x, y, z;
float roll = 0; // Rolamento

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
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    // Calcular o rolamento (roll) usando atan2
    roll = atan2(y, sqrt(x * x + z * z)) * 180 / PI; // Converter de radianos para graus

    Serial.print("Roll (rolamento): ");
    Serial.print(roll);
    Serial.println(" degrees");
  }
  delay(1000);
}
