#ifndef ACCELEROMETER_GENERIC_H
#define ACCELEROMETER_GENERIC_H

#include "i2c_generic.h" // Include the i2c class definition
#include <Arduino.h>
#include <Wire.h>
#include "Q16_16.h"

class Accelerometer {
public:
    Accelerometer(i2c& i2cObj); // Constructor takes an i2c object
    ~Accelerometer();

    int begin();
    void end();
    void setContinuousMode();
    bool getAngle();
    int accelerationAvailable();
    float calculateRoll();

private:
    bool continuousMode;
    i2c* _i2c; // Pointer to the i2c object
    Q16_16 x, y, z; // Measured Values

    
    // Helper functions
    int readRegister(uint8_t slaveAddress, uint8_t address);
    int readRegisters(uint8_t slaveAddress, uint8_t address, uint8_t* data, size_t length);
    int writeRegister(uint8_t slaveAddress, uint8_t address, uint8_t value);
};

#endif
