#ifndef INFRARED_GENERIC_H
#define INFRARED_GENERIC_H

#include <Arduino.h>

// DataPoint Class
class DataPoint {
private:
    float distance;
    float value;

public:
    DataPoint() : distance(0), value(0) {}  // Default constructor
    DataPoint(float dist, float val) : distance(dist), value(val) {}

    float getDistance() const { return distance; }
    float getValue() const { return value; }

    void display() const {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.print(", Value: ");
        Serial.println(value);
    }
};

// InfraredSensor Class
class InfraredSensor {
private:
    int voltPin;
    float sensorVolt;
    float sensorValue;
    DataPoint dataPoints[10];  // Fixed-size array
    size_t dataPointCount;

    static constexpr float VOLTAGE_REF = 3.3;  // Configurable reference voltage
    static constexpr int MY_ADC_RESOLUTION = 1023; // Avoid conflict with ADC_RESOLUTION macro

public:
    InfraredSensor(int pin);
    ~InfraredSensor();

    void begin();
    void measure();
    void conversion(float value);
    float getVolt();
    float getValue();
    const DataPoint& getDataPoint(size_t index) const;
    size_t size() const;
    void addDataPoint(float distance);
};

#endif
