#include "Infrared_generic.h"

static InfraredSensor* instance;


//Usar o pin 19 (A0) a 26 (A6)
//Constructor 
InfraredSensor::InfraredSensor(int voltPin)
    :   inPin(voltPin), sensorVolt(0), sensorValue(0), offset(0){
            instance=this;
}

InfraredSensor::~InfraredSensor()
{
}

// Initialize the infrared system
void InfraredSensor::begin(){
    pinMode(inPin, INPUT);
}

void InfraredSensor::measure(){
    sensorValue = analogRead(voltPin);
    conversion(sensorValue);
}

void InfraredSensor::conversion(float value){
    sensorVolt = value * (3.3 / 1023.0);
    return;
}

float InfraredSensor::getVolt(){
    return sensorVolt;
}

float InfraredSensor::getValue(){
    return sensorValue;
}

const DataPoint& getDataPoint(size_t index) const {
    if (index >= dataPoints.size()) {
        throw std::out_of_range("Index out of range");
    }
    return dataPoints[index];
}

size_t size() const {
    return dataPoints.size();
}

void addDataPoint(float distance) {
    measure();
    DataPoint newPoint(distance, sensorValue);

    // Find the position where the new point should be inserted
    auto it = std::lower_bound(dataPoints.begin(), dataPoints.end(), newPoint,
                               [](const DataPoint& a, const DataPoint& b) {
                                   return a.getDistance() < b.getDistance();
                               });

    // Insert the new point at the correct position
    dataPoints.insert(it, newPoint);
}