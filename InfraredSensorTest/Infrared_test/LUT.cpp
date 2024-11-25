#include "LUT.h"

// DataPoint display function
void DataPoint::display() const {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(", Value: ");
    Serial.println(value);
}

// Add a point to the LUT
void LUT::addPoint(float distance, float value) {
    points.emplace_back(distance, value);
}

// Calculate slopes and intercepts after points are added
void LUT::calculateSlopesAndIntercepts() {
    slopes.clear();
    intercepts.clear();

    for (size_t i = 0; i < points.size() - 1; ++i) {
        float deltaDistance = points[i + 1].getDistance() - points[i].getDistance();
        float slope = deltaDistance != 0 ? (points[i + 1].getValue() - points[i].getValue()) / deltaDistance : 0;
        slopes.push_back(slope);
        intercepts.push_back(points[i].getValue() - slope * points[i].getDistance());
    }
}

// Interpolate distance for a given sensor value
float LUT::getDistance(float valueRead) const {
    for (size_t i = 0; i < points.size() - 1; ++i) {
        if (points[i].getValue() <= valueRead && valueRead <= points[i + 1].getValue()) {
            return slopes[i] * valueRead + intercepts[i];
        }
    }

    if (!slopes.empty()) {
        return slopes.back() * valueRead + intercepts.back();
    }

    return 0.0;
}

// Display LUT details
void LUT::display() const {
    for (size_t i = 0; i < points.size(); ++i) {
        Serial.print("Point ");
        Serial.print(i);
        Serial.print(": ");
        points[i].display();
        if (i < slopes.size()) {
            Serial.print("  Slope: ");
            Serial.print(slopes[i]);
            Serial.print(", Intercept: ");
            Serial.println(intercepts[i]);
        }
    }
}
