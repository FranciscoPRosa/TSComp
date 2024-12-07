#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>
#include "Q16_16.h"

#define USS_MEANS 32

class UltrasonicSensor {
  private:
      int trigPin;
      int echoPin;
      volatile unsigned long startTime;
      volatile unsigned long endTime;

      static void echoISRWrapper(); // Static wrapper for ISR
      void sendPulse();

  public:
     /**
      * @brief Construct a new Ultrasonic Sensor object
      * 
      * @param trig The trigger pin
      * @param echo The echo pin
      */
      UltrasonicSensor(int trig, int echo);

      /**
       * @brief Begin the Ultrasonic Sensor and initiates the pulse-recieve sequence measuring sequance
       * 
       * @warning This means that periodically the main code will be interrupted when a pulse is received. This interrupt will generate a new pulse,
       * and return the execution context until the next pulse is received.
       */
      void begin();
      
      /**
       * @brief Get the Distance object
       * 
       * Each time there is a measurment, that value is placed in a circular buffer. When this functions is called, the average value of this buffer is used to return the distance.
       * 
       * @return float The distance measured by the Ultrasonic Sensor
       */
      float getDistance();
};

#endif // ULTRASONIC_SENSOR_H
