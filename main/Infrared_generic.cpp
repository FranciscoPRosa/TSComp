#include "Infrared_generic.h"

// Constructor: initializes the LUT with predefined points
InfraredSensor::InfraredSensor(int pin)
    : voltPin(pin), sensorVolt(0), sensorValue(0), distanceMeas(0), offsetColor(0) {
    // initialization of the values for the curve
    // if change is needed cut or add the necessary values
    curveLUT.addPoint(0, 986);
    curveLUT.addPoint(1, 964);
    curveLUT.addPoint(2, 633);
    curveLUT.addPoint(3, 474);
    curveLUT.addPoint(4, 361);
    curveLUT.addPoint(5, 299);
    curveLUT.addPoint(6, 257);
    curveLUT.addPoint(7, 236);
    curveLUT.addPoint(8, 220);
    curveLUT.addPoint(9, 209);
    curveLUT.addPoint(10, 194);
    curveLUT.addPoint(12, 179);
    curveLUT.addPoint(16, 163);
    curveLUT.addPoint(20, 155);
    curveLUT.calculateSlopesAndIntercepts();
    // using Cleaver (Metal) for the first approximation
    // Distances taken for the curve: 0,1,2,3,4,5,8,10,12,16,20
    colorOffset.addPoint(0, 980);
    colorOffset.addPoint(1, 645);
    colorOffset.addPoint(2, 450);
    colorOffset.addPoint(3, 328);
    colorOffset.addPoint(4, 273);
    colorOffset.addPoint(5, 236);
    colorOffset.addPoint(6, 213);
    colorOffset.addPoint(7, 197);
    colorOffset.addPoint(8, 185);
    colorOffset.addPoint(9, 176);
    colorOffset.addPoint(10, 171);
    colorOffset.addPoint(12, 162);
    colorOffset.addPoint(16, 152);
    colorOffset.addPoint(20, 147);
    colorOffset.calculateSlopesAndIntercepts();

    // Black plastic for the other calibration curve
    blackOffset.addPoint(0,246);
    blackOffset.addPoint(1,199);
    blackOffset.addPoint(2,177);
    blackOffset.addPoint(3,163);
    blackOffset.addPoint(4,156);
    blackOffset.addPoint(5,150);
    blackOffset.addPoint(6,147);
    blackOffset.addPoint(7,145);
    blackOffset.addPoint(8,143);
    blackOffset.addPoint(9,142);
    blackOffset.addPoint(10,141);
    blackOffset.addPoint(12,139);
    blackOffset.addPoint(16,138);
    blackOffset.addPoint(20,137);
    blackOffset.calculateSlopesAndIntercepts();

    offsetCurve = &curveLUT;
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
    distanceMeas = (*offsetCurve).getDistance(sensorValue);
}

// Get the calculated distance
float InfraredSensor::getDistanceMeasure() const {
    return distanceMeas;
}

void InfraredSensor::updateOffset(float usDistance){
    measure();
    int ogValue = abs(curveLUT.getCorrValue(usDistance)-sensorValue);
    int colorValue = abs(colorOffset.getCorrValue(usDistance)-sensorValue);
    int blackValue = abs(blackOffset.getCorrValue(usDistance)-sensorValue);

    if(min(ogValue,colorValue)==ogValue){
        if(min(ogValue,blackValue)==ogValue){
            offsetCurve = &curveLUT;
            Serial.println("Using White!");
        } else {
            offsetCurve = &blackOffset;
            Serial.println("Uaing Black!");
        }
    } else {
        if(min(colorValue, blackValue)==colorValue){
            offsetCurve = &colorOffset;
            Serial.println("Uaing Color!");
        } else {
            offsetCurve = &blackOffset;
            Serial.println("Uaing Black!");
        }
    }   
}
