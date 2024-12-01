#ifndef LUT_H
#define LUT_H

#include <Arduino.h>
#include <vector>

#define NUM 25

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
};

// LUT Class
class LUT {
private:
    DataPoint points[NUM];
    float slopes[NUM - 1];
    float intercepts[NUM - 1];

public:
    void addPoint(float distance, float value);
    void calculateSlopesAndIntercepts();
    float getDistance(float valueRead) const;
    float getCorrValue(float distanceRead) const;
};

#endif