#include "LUT.h"

// Add a point to the LUT
void LUT::addPoint(float distance, float value) {
    static size_t index = 0;
    if (index < NUM) {
        points[index] = DataPoint(distance, value);
        index++;
    }else{
        Serial.println("LUT is full!");
    }
}

// Calculate slopes and intercepts after points are added
void LUT::calculateSlopesAndIntercepts() {
    for (size_t i = 0; i < NUM - 1; ++i) {
        float slope = (points[i + 1].getDistance() - points[i].getDistance()) / (points[i + 1].getValue() - points[i].getValue());
        slopes[i] = slope;
        intercepts[i] = points[i].getDistance() - slope * points[i].getValue();
    }
}

// Interpolate distance for a given sensor value
float LUT::getDistance(float valueRead) const {
    if(valueRead < points[NUM - 1].getValue()) {
        return points[NUM - 1].getDistance();
    }else if(valueRead > points[0].getValue()) {
        return points[0].getDistance();
    }
    for (size_t i = 0; i < NUM - 1; ++i) {
        if (points[i].getValue() >= valueRead && valueRead >= points[i + 1].getValue()) {
            return slopes[i] * valueRead + intercepts[i];
        }
    }
    return 0.0;
}

// Interpolate sensor value for a given distance
float LUT::getCorrValue(float distanceRead) const {
    // Check if the distance is outside the LUT range
    if (distanceRead < points[NUM - 1].getDistance()) {
        return points[NUM - 1].getValue();
    } else if (distanceRead > points[0].getDistance()) {
        return points[0].getValue();
    }
    
    // Iterate through the points to find the correct segment
    for (size_t i = 0; i < NUM - 1; ++i) {
        if (points[i].getDistance() >= distanceRead && distanceRead >= points[i + 1].getDistance()) {
            // Reverse interpolation formula: value = (distance - intercept) / slope
            return (distanceRead - intercepts[i]) / slopes[i];
        }
    }
    return 0.0; // Default value if no segment is found
}