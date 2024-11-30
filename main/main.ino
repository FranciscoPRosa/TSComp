#include "BLEServer_generic.h"
#include "UltrasonicSensor.h"

#include <Wire.h>
#include "i2c_generic.h"
#include "Accelerometer_generic.h"

#include "Infrared_generic.h"

// Ultrasonic Sensor pins
#define TRIG_PIN 9
#define ECHO_PIN 2
// BLE Server variables
#define BLE 0
#define STATUS_LED LED_BUILTIN
#define BLE_NAME "Ultrasonic Sensor"
#define BUFFER_SIZE 30

// Infrared definitions
#define INPUT_PIN 14

//General definitions
#define DEGREES_TO_RADIANS 0.0174532925

// BLE Server Objects
GenericBLEServer bleServer(BLE_NAME, STATUS_LED);
BLEService ultrasonicService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEStringCharacteristic distanceCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, BUFFER_SIZE);

// Ultrasonic Sensor Object
UltrasonicSensor ultrasonic(TRIG_PIN, ECHO_PIN);

// Accelerometer Object
i2c i2cObj(Wire1); // Create i2c object with default Wire instance
Accelerometer accel(i2cObj); // Pass i2c object to Accelerometer

// Infrared Sensor
InfraredSensor sensor(INPUT_PIN);

float compensateAngle(float distance){
  //return distance;
  float angle = 0;
  if(accel.accelerationAvailable()) angle = accel.getAngle(angle);
  return distance * cos(angle * DEGREES_TO_RADIANS);
}

float measure(){
  float distanceIR = 0, distanceUSS = 0;

  distanceUSS = ultrasonic.getDistance();
  sensor.measure();
  sensor.calculateDistance();
  distanceIR = sensor.getDistanceMeasure();

  //Serial.println("USS: " + String(distanceUSS) + " | IR: " + String(distanceIR) + " | Mean: " + String(compensateAngle(distanceIR + distanceUSS) / 2.0));

  if(distanceIR < 4.0) return compensateAngle(distanceIR);
  if(distanceUSS > 13.5) return compensateAngle(distanceUSS);
  return compensateAngle((distanceIR + distanceUSS) / 2.0);
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
  sensor.begin();
}

void loop() {
  float distance = 0;
  char buffer[BUFFER_SIZE];

#if BLE
  bleServer.connect();
  while (bleServer.isConnected()) {
    distance = measure();

    sprintf(buffer, "%.2f cm", distance);
    Serial.println(buffer);
    distanceCharacteristic.writeValue(buffer);
  }
#else
  distance = measure();

  sprintf(buffer, "%.2f cm", distance);
  Serial.println(buffer);
  delay(50);
#endif
}
