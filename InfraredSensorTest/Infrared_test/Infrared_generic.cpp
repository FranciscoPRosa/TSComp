#include "Infrared_generic.h"

// Constructor: initializes the LUT with predefined points
InfraredSensor::InfraredSensor(int pin)
    : voltPin(pin), sensorVolt(0), sensorValue(0), distanceMeas(0) {
    // initialization of the values for the curve
    // if change is needed cut or add the necessary values
    curveLUT.addPoint(0, 963);
    curveLUT.addPoint(2, 660);
    curveLUT.addPoint(4, 353);
    curveLUT.addPoint(6, 270);
    curveLUT.addPoint(8, 222);
    curveLUT.addPoint(10, 201);
    curveLUT.addPoint(12, 194);
    curveLUT.addPoint(14, 182);
    curveLUT.addPoint(16, 172);
    curveLUT.addPoint(18, 113);
    curveLUT.addPoint(20, 110);
    curveLUT.calculateSlopesAndIntercepts();
}

InfraredSensor::~InfraredSensor() {}

void InfraredSensor::begin() {
    pinMode(voltPin, INPUT);
}

// Measure the sensor value by averaging readings
void InfraredSensor::measure() {
    long values = 0;
    for (int i = 0; i < 128; i++) {
        values += analogRead(voltPin);
    }
    sensorValue = values >> 7;
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
    distanceMeas = curveLUT.getDistance(sensorValue);
}

// Get the calculated distance
float InfraredSensor::getDistanceMeasure() const {
    return distanceMeas;
}
