#ifndef INFRARED_SENSOR_H
#define INFRARED_SENSOR_H

#include <Arduino.h>
#include <algorithm>

// Define the DataPoint class
class DataPoint {
private:
    float distance;
    float value;

public:
    // Constructor
    DataPoint(float d, float v) : distance(d), value(v) {}

    // Getter for distance
    float getDistance() const { return distance; }

    // Getter for value
    float getValue() const { return value; }

    // Display method
    void display() const {
        std::cout << "Distance: " << distance << ", Value: " << value << std::endl;
    }
};


class InfraredSensor{
    public:
        int voltPin;
        InfraredSensor(int voltPin);
        ~InfraredSensor();
        void begin();
        void measure();
        void getVolt();
        void getValue();
        void addDataPoint(float distance, float value);
        // Method to get the total number of DataPoints
        size_t size() const;
        // Access to a specific DataPoint
        const DataPoint& getDataPoint(size_t index) const;

    private:
        int voltPin;
        float sensorVolt;
        float sensorValue;
        std::vector<DataPoint> dataPoints; // Vector of DataPoint objects with the curve 
        void conversion(float value);
};

#endif