#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

#define USS_MEANS 32

class UltrasonicSensor {
  private:
      int trigPin;
      int echoPin;
      volatile unsigned long startTime;
      volatile unsigned long endTime;
      volatile float distance;
      const float speedOfSound = 343.0; // Speed of sound in air (m/s)

      static void echoISRWrapper(); // Static wrapper for ISR
      void sendPulse();

  public:
      UltrasonicSensor(int trig, int echo);
      // Inicializes the UltrasonicSensor class
      //and sets in motion the pulse->interrupt->pulse sequence
      void begin();
      // Returns the last calculated distance in float
      float getDistance();
};

#endif // ULTRASONIC_SENSOR_H
