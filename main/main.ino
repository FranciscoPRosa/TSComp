#include "BLEServer_generic.h"
#include "UltrasonicSensor.h"

#include <Wire.h>
#include "i2c_generic.h"
#include "Accelerometer_generic.h"

// Ultrasonic Sensor pins
#define TRIG_PIN 9
#define ECHO_PIN 2
// BLE Server variables
#define BLE 0
#define STATUS_LED LED_BUILTIN
#define BLE_NAME "Ultrasonic Sensor"
#define BUFFER_SIZE 30

// BLE Server Objects
GenericBLEServer bleServer(BLE_NAME, STATUS_LED);
BLEService ultrasonicService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEStringCharacteristic distanceCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, BUFFER_SIZE);

// Ultrasonic Sensor Object
UltrasonicSensor ultrasonic(TRIG_PIN, ECHO_PIN);

// Accelerometer Object
i2c i2cObj(Wire1); // Create i2c object with default Wire instance
Accelerometer accel(i2cObj); // Pass i2c object to Accelerometer

float compensateAngle(float distance){
  float angle = 0;
  if(accel.accelerationAvailable()) angle = accel.getAngle(angle);
  return distance * cos(angle * 0.0174532925);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize the accelerometer
  if (!accel.begin()) {
        Serial.println("Failed to initialize accelerometer!");
        while (1);
    }
  Serial.println("Accelerometer initialized.");

#if BLE
  // Begin the BLE server
  Serial.println("Starting BLE Server...");
  bleServer.begin();
  
  ultrasonicService.addCharacteristic(distanceCharacteristic);
  bleServer.setService(ultrasonicService);
  bleServer.advertise();
#endif

  // Begin the measuring sequence
  ultrasonic.begin();
}

void loop() {
  float distance = 0;
  char buffer[BUFFER_SIZE];

  float distance_angle = 0;

#if BLE
  bleServer.connect();
  while (bleServer.isConnected()) {
    distance = ultrasonic.getDistance();
    distance_angle = compensateAngle(distance);
    sprintf(buffer, "%.2f | %.2f cm | angle %.2f", distance_angle, distance, angle);
    Serial.println(buffer);
    distanceCharacteristic.writeValue(buffer);
  }
#else
  distance = ultrasonic.getDistance();
  distance_angle = compensateAngle(distance);
  sprintf(buffer, "%.2f | %.2f cm | angle %.2f", distance_angle, distance, angle);
  Serial.println(buffer);
  delay(50);
#endif
}
