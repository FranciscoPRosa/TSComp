#include "BLEServer_generic.h"
#include "UltrasonicSensor.h"

#define TRIG_PIN 9
#define ECHO_PIN 2
#define STATUS_LED LED_BUILTIN
#define BLE_NAME "Ultrasonic Sensor"
#define BUFFER_SIZE 20

GenericBLEServer bleServer(BLE_NAME, STATUS_LED);
UltrasonicSensor ultrasonic(TRIG_PIN, ECHO_PIN);

  BLEService ultrasonicService("19B10000-E8F2-537E-4F6C-D104768A1214");
  BLEStringCharacteristic distanceCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, BUFFER_SIZE);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Starting BLE Server...");
  // Begin the BLE server
  bleServer.begin();
  
  ultrasonicService.addCharacteristic(distanceCharacteristic);
  bleServer.setService(ultrasonicService);
  bleServer.advertise();
  
  // Begin the measuring sequence
  ultrasonic.begin();
}

void loop() {
  bleServer.connect();
  while (bleServer.isConnected()) {
    float distance = ultrasonic.getDistance();
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "%.2f cm", distance);
    Serial.println(buffer);
    distanceCharacteristic.writeValue(buffer);
  }
}

/*#include <Arduino_LSM9DS1.h>
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
}*/
