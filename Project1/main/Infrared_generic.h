#ifndef INFRARED_SENSOR_H
#define INFRARED_SENSOR_H

#include <Arduino.h>
#include "LUT.h"

#define NUM_LUTS 3

class InfraredSensor {
private:
    int voltPin;
    float sensorVolt;
    int sensorValue;
    float distanceMeas;
    int offsetColor;
    LUT arr[NUM_LUTS];
    LUT *offsetCurve;

    static constexpr float VOLTAGE_REF = 3.3;
    static constexpr int MY_ADC_RESOLUTION = 1023;

public:
    InfraredSensor(int pin);
    ~InfraredSensor();

    void begin();
    void measure();
    float getVolt() const;
    int getValue() const;
    void calculateDistance();
    float getDistanceMeasure() const;
    void updateOffset(float usDistance);
};

#endif
