#ifndef LUT_H
#define LUT_H

#include <Arduino.h>
#include <vector>

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
    void display() const;
};

// LUT Class
class LUT {
private:
    std::vector<DataPoint> points;
    std::vector<float> slopes;
    std::vector<float> intercepts;

public:
    void addPoint(float distance, float value);
    void calculateSlopesAndIntercepts();
    float getDistance(float valueRead) const;
    void display() const;
};

#endif
