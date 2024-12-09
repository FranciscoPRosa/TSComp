#include "Infrared_generic.h"

// Constructor: initializes the LUT with predefined points
InfraredSensor::InfraredSensor(int pin)
    : voltPin(pin), sensorVolt(0), sensorValue(0), distanceMeas(0), offset(0) {
    // initialization of the values for the curve
    // if change is needed cut or add the necessary values
    curveLUT.addPoint(0, 931);
    curveLUT.addPoint(1, 824);
    curveLUT.addPoint(2, 623);
    curveLUT.addPoint(3, 441);
    curveLUT.addPoint(4, 345);
    curveLUT.addPoint(5, 290);
    curveLUT.addPoint(6, 254);
    curveLUT.addPoint(7, 233);
    curveLUT.addPoint(8, 216);
    curveLUT.addPoint(9, 201);
    curveLUT.addPoint(10, 191);
    curveLUT.addPoint(11, 184);
    curveLUT.addPoint(12, 177);
    curveLUT.addPoint(13, 174);
    curveLUT.addPoint(14, 168);
    curveLUT.addPoint(15, 166);
    curveLUT.addPoint(16, 164);
    curveLUT.addPoint(17, 161);
    curveLUT.addPoint(18, 159);
    curveLUT.addPoint(19, 158);
    curveLUT.addPoint(20, 157);
    curveLUT.addPoint(25, 153);
    curveLUT.addPoint(30, 150);
    curveLUT.addPoint(40, 147);
    curveLUT.addPoint(50, 146);
    curveLUT.calculateSlopesAndIntercepts();
}

InfraredSensor::~InfraredSensor() {}

void InfraredSensor::begin() {
    pinMode(voltPin, INPUT);
}

// Measure the sensor value by averaging readings
void InfraredSensor::measure() {
    long values = 0;
    for (int i = 0; i < 1024; i++) {
        values += analogRead(voltPin);
    }
    sensorValue = values >> 10;
    sensorVolt = sensorValue * (VOLTAGE_REF / MY_ADC_RESOLUTION);
}

// Get sensor voltage
float InfraredSensor::getVolt() const {
    return sensorVolt;
}

// Get raw sensor value
float InfraredSensor::getValue() const {
    return sensorValue;
}

// Calculate distance based on the measured sensor value
void InfraredSensor::calculateDistance() {
    distanceMeas = offset + curveLUT.getDistance(sensorValue);
}

// Get the calculated distance
float InfraredSensor::getDistanceMeasure() const {
    return distanceMeas;
}

float InfraredSensor::updateOffset(float usDistance){
  measure();
  calculateDistance();
  offset = usDistance - distanceMeas;
}
