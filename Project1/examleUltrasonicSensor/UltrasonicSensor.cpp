#include "UltrasonicSensor.h"

static UltrasonicSensor* instance; // Singleton instance for ISR reference
int sent = 0;

// Constructor
UltrasonicSensor::UltrasonicSensor(int trig, int echo)
    : trigPin(trig), echoPin(echo), startTime(0), endTime(0), distance(0) {
    instance = this; // Assign this instance for ISR
}

// Send a pulse from the trigger pin
void UltrasonicSensor::sendPulse() {
    sent = 1;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
}

// Static ISR wrapper to access class methods
void UltrasonicSensor::echoISRWrapper() {
    if (instance) {
        if (digitalRead(instance->echoPin) == HIGH) {
            instance->startTime = micros();
        } else {
          if(sent){
            sent = 0;
            instance->endTime = micros();
            unsigned long duration = instance->endTime - instance->startTime;
            instance->distance = (duration / 2.0) * (instance->speedOfSound / 1000000.0) * 100;
            instance->sendPulse();
          }
        }
    }
}

// Initialize the ultrasonic sensor
void UltrasonicSensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(echoPin), UltrasonicSensor::echoISRWrapper, CHANGE);
    sendPulse();
}

// Get the latest distance measurement
float UltrasonicSensor::getDistance() {
    return distance;
}
