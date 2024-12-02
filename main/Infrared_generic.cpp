#include "Infrared_generic.h"

// Constructor: initializes the LUT with predefined points
InfraredSensor::InfraredSensor(int pin)
    : voltPin(pin), sensorVolt(0), sensorValue(0), distanceMeas(0), offsetColor(0) {
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
    // using Cleaver (Metal) for the first approximation
    // Distances taken for the curve: 0,1,2,3,4,5,8,10,12,16,20
    colorOffset.addPoint(995, 931);
    colorOffset.addPoint(981, 824);
    colorOffset.addPoint(655, 623);
    colorOffset.addPoint(447, 441);
    colorOffset.addPoint(374, 345);
    colorOffset.addPoint(319, 290);
    colorOffset.addPoint(241, 216);
    colorOffset.addPoint(215, 191);
    colorOffset.addPoint(195, 177);
    colorOffset.addPoint(177, 164);
    colorOffset.addPoint(166, 157);
    colorOffset.calculateSlopesAndIntercepts();

    // Black plastic for the other calibration curve
    blackOffset.addPoint(385,931);
    blackOffset.addPoint(250,824);
    blackOffset.addPoint(203,623);
    blackOffset.addPoint(177,441);
    blackOffset.addPoint(166,345);
    blackOffset.addPoint(161,290);
    blackOffset.addPoint(156,216);
    blackOffset.addPoint(153,191);
    blackOffset.addPoint(152,177);
    blackOffset.addPoint(154,164);
    blackOffset.addPoint(151,157);
    blackOffset.calculateSlopesAndIntercepts();

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
int InfraredSensor::getValue() const {
    return sensorValue;
}

// Calculate distance based on the measured sensor value
void InfraredSensor::calculateDistance() {
    int valueFinal = offsetCurve.getDistance(sensorValue+offsetColor);
    distanceMeas = curveLUT.getDistance(sensorValue);
}

// Get the calculated distance
float InfraredSensor::getDistanceMeasure() const {
    return distanceMeas;
}

void InfraredSensor::updateOffset(float usDistance){
    int ogValue, calibValue;
    measure();
    calculateDistance();
    ogValue = curveLUT.getCorrValue(usDistance);
    if((ogValue-sensorValue)<0){
        offsetCurve = blackOffset;
    } else {
        offsetCurve = colorOffset;
    }
    calibValue = offsetCurve.getCorrValue(ogValue);
    offsetColor = calibValue - sensorValue;
}
