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
#define BLE 1                        // Set to 1 to enable BLE
#define STATUS_LED LED_BUILTIN       // Status LED pin - built-in LED will activate when BLE is connected
#define BLE_NAME "SensorSystemG4"    // BLE Device Name detected by other devices
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
// Since the Model used is the Arduino 33 Nano BLE, it must be Wire1 instead of Wire only
i2c i2cObj(Wire1); // Create i2c object with default Wire instance
Accelerometer accel(i2cObj); // Pass i2c object to Accelerometer

// Infrared Sensor
InfraredSensor sensor(INPUT_PIN);

/**
 * @brief Compensates the distance measured by the sensors using the angle
 * The formula is: new = distance * cos(angle)
 * 
 * @param distance The distance measured by the sensors
 * @return float The compensated distance
 */
float compensateAngle(float distance){
  //return distance;
  static float angle = 0;
  if(accel.accelerationAvailable()){
    accel.getAngle();
    angle = accel.calculateRoll();
  }
  return distance * cos(angle * DEGREES_TO_RADIANS);
}

/**
 * @brief Measures the distance using the Infrared and Ultrasonic sensor
 * This is where all the important logic happens! The moment this function is called
 * the sensors will return the values they have on their internal variables and those values
 * will be used to calculate the distance. The distance is then compensated by the angle
 * measured by the accelerometer.
 * 
 * Notice that the distance is calculated keeping in mind the range of the sensors. If the
 * distance is less than 4.0 cm, the distance measured by the Infrared sensor is returned.
 * If the distance is greater than 13.5 cm, the distance measured by the Ultrasonic sensor is
 * returned. If the distance is between 4.0 and 13.5 cm, the average of the two distances is
 * returned.
 * 
 * @return float The distance measured and compensated
 */
float measure(){
  float distanceIR = 0, distanceUSS = 0;

  // Get angle for the Serial.println (for show)
  static float angle = 0;
  if(accel.accelerationAvailable()){
    accel.getAngle();
    angle = accel.calculateRoll();
  }

  // Getting the distance that the UltraSonic Sensor measures
  distanceUSS = ultrasonic.getDistance();

  // If the distance is between 3.8 and 4.2 cm, update the offset, causing our sensor to update
  // it's current Look-Up Table depending on the material. This interval was selected because it gives
  // good confidence in both sensors.
  if(distanceUSS < 4.2 && distanceUSS > 3.8) sensor.updateOffset(distanceUSS);

  // Getting the distance that the Infrared Sensor measures
  sensor.measure();
  sensor.calculateDistance();
  distanceIR = sensor.getDistanceMeasure();

  Serial.println("USS: " + String(distanceUSS) + " | IR: " + String(distanceIR) + " | Angle: " + String(angle));

  // If the distance is less than 4.0 cm, return the compensated distance measured by the Infrared sensor
  if(distanceIR < 4.0) return compensateAngle(distanceIR);
  // If the distance is greater than 13.5 cm, return the compensated distance measured by the Ultrasonic sensor
  if(distanceUSS > 13.5) return compensateAngle(distanceUSS);
  // If the distance is between 4.0 and 13.5 cm, return the compensated average of the two distances
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
    // Writing in the BLE characteristic/register
    distanceCharacteristic.writeValue(buffer);
  }
#else
  distance = measure();

  sprintf(buffer, "%.2f cm", distance);
  Serial.println(buffer);
  delay(50);
#endif
}
