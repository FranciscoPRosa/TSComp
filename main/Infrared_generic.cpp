#include "Infrared_generic.h"

// Constructor: initializes the LUT with predefined points
InfraredSensor::InfraredSensor(int pin)
    : voltPin(pin), sensorVolt(0), sensorValue(0), distanceMeas(0), offsetColor(0) {
    // initialization of the values for the curve
    // if change is needed cut or add the necessary values
    arr[0].addPoint(0, 997);
    arr[0].addPoint(1, 719);
    arr[0].addPoint(2, 443);
    arr[0].addPoint(3, 289);
    arr[0].addPoint(4, 203);
    arr[0].addPoint(5, 157);
    arr[0].addPoint(6, 131);
    arr[0].addPoint(7, 109);
    arr[0].addPoint(8, 97);
    arr[0].addPoint(9, 86);
    arr[0].addPoint(10, 78);
    arr[0].addPoint(12, 68);
    arr[0].addPoint(16, 57);
    arr[0].addPoint(20, 52);
    arr[0].calculateSlopesAndIntercepts();
    // using Cleaver (Metal) for the first approximation
    // Distances taken for the curve: 0,1,2,3,4,5,8,10,12,16,20
    arr[1].addPoint(0, 997);
    arr[1].addPoint(1, 562);
    arr[1].addPoint(2, 351);
    arr[1].addPoint(3, 233);
    arr[1].addPoint(4, 168);
    arr[1].addPoint(5, 134);
    arr[1].addPoint(6, 111);
    arr[1].addPoint(7, 97);
    arr[1].addPoint(8, 84);
    arr[1].addPoint(9, 77);
    arr[1].addPoint(10, 71);
    arr[1].addPoint(12, 63);
    arr[1].addPoint(16, 55);
    arr[1].addPoint(20, 52);
    arr[1].calculateSlopesAndIntercepts();

    // Black plastic for the other calibration curve
    arr[2].addPoint(0, 121);
    arr[2].addPoint(1, 82);
    arr[2].addPoint(2, 64);
    arr[2].addPoint(3, 56);
    arr[2].addPoint(4, 51);
    arr[2].addPoint(5, 49);
    arr[2].addPoint(6, 47);
    arr[2].addPoint(7, 46);
    arr[2].addPoint(8, 45);
    arr[2].addPoint(9, 45);
    arr[2].addPoint(10, 44);
    arr[2].addPoint(12, 44);
    arr[2].addPoint(16, 43);
    arr[2].addPoint(20, 42);
    arr[2].calculateSlopesAndIntercepts();

    offsetCurve = &arr[0];
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

    int min = 100000;
    int selected_lut = 0;

    for(int i = 0; i < NUM_LUTS; i++){
      int aux = abs(arr[i].getCorrValue(usDistance)-sensorValue);
      if(aux < min){
        min = aux;
        selected_lut = i;
      }
    }
    Serial.print("Using: "); Serial.println(selected_lut);
    offsetCurve = &arr[selected_lut];  
}
