#ifndef INFRARED_SENSOR_H
#define INFRARED_SENSOR_H

#include <Arduino.h>
#include "LUT.h"

class InfraredSensor {
private:
    int voltPin;
    float sensorVolt;
    float sensorValue;
    float distanceMeas;
    LUT curveLUT;

    static constexpr float VOLTAGE_REF = 3.3;
    static constexpr int MY_ADC_RESOLUTION = 1023;

public:
    InfraredSensor(int pin);
    ~InfraredSensor();

    void begin();
    void measure();
    float getVolt() const;
    float getValue() const;
    void calculateDistance();
    float getDistanceMeasure() const;
};

#endif
