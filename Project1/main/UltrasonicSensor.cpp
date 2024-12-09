#include "UltrasonicSensor.h"

static UltrasonicSensor* instance; // Singleton instance for ISR reference
int sent = 0;
unsigned long distArr[USS_MEANS] = {0};
int distIndex = 0;

Q16_16 speedOfSound = Q16_16::fromFloat(0.01715);  // Speed of sound in m/s

// Constructor
UltrasonicSensor::UltrasonicSensor(int trig, int echo)
    : trigPin(trig), echoPin(echo), startTime(0), endTime(0) {
    instance = this; // Assign this instance for ISR
}

// Send a pulse from the trigger pin
void UltrasonicSensor::sendPulse() {
    sent = 1;                       // Set the sent flag
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
}

// Static ISR wrapper to access class methods
void UltrasonicSensor::echoISRWrapper() {
    if (instance) {
        // If the echo pin is high, start the timer
        if (digitalRead(instance->echoPin) == HIGH) {
            instance->startTime = micros();
        // If the echo pin is low, stop the timer and calculate the duration
        } else {
          if(sent){
            sent = 0;
            instance->endTime = micros();
            unsigned long duration = instance->endTime - instance->startTime;
            instance->sendPulse(); // Start the next pulse
            distArr[distIndex] = duration;
            distIndex++; distIndex %= USS_MEANS;
          }
        }
    }
}

// Initialize the ultrasonic sensor
void UltrasonicSensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(echoPin), UltrasonicSensor::echoISRWrapper, CHANGE);
    sendPulse(); // Start the first pulse
}

// Get the latest distance measurement
float UltrasonicSensor::getDistance() {
    unsigned long aux = 0;
    for(int i = 0; i < USS_MEANS; i++){
      aux += distArr[i];
    }
    return (Q16_16::fromInt(aux / USS_MEANS) * speedOfSound).toFloat();
}
