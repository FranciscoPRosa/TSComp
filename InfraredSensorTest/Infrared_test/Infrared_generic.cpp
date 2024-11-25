#include "Infrared_generic.h"

// Constructor
InfraredSensor::InfraredSensor(int pin)
    : voltPin(pin), sensorVolt(0), sensorValue(0), dataPointCount(0) {}

// Destructor
InfraredSensor::~InfraredSensor() {}

// Initialize the sensor
void InfraredSensor::begin() {
    pinMode(voltPin, INPUT);
}

// Measure the raw sensor value
void InfraredSensor::measure() {
    long values;
    // Average to filter some noise
    for(int i=0; i<128; i++){
      values += analogRead(voltPin);
    }
    sensorValue = values>>7;
    conversion(sensorValue);
}

// Convert the raw value to a voltage
void InfraredSensor::conversion(float value) {
    sensorVolt = value * (VOLTAGE_REF / MY_ADC_RESOLUTION);
}

// Get the current sensor voltage
float InfraredSensor::getVolt() {
    return sensorVolt;
}

// Get the current raw sensor value
float InfraredSensor::getValue() {
    return sensorValue;
}

// Access a specific DataPoint by index
const DataPoint& InfraredSensor::getDataPoint(size_t index) const {
    if (index >= dataPointCount) {
        Serial.println("Error: Index out of range");
        static DataPoint invalidDataPoint(-1, -1);  // Sentinel value
        return invalidDataPoint;
    }
    return dataPoints[index];
}

// Get the number of stored DataPoints
size_t InfraredSensor::size() const {
    return dataPointCount;
}

// Add a new DataPoint to the array
void InfraredSensor::addDataPoint(float distance) {
    if (dataPointCount >= 20) {
        Serial.println("Error: DataPoint storage full");
        return;
    }
    measure();
    dataPoints[dataPointCount++] = DataPoint(distance, sensorVolt);
}
