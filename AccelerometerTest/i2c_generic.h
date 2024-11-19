#ifndef I2C_GENERIC_H
#define I2C_GENERIC_H

#include <Wire.h>

class i2c {
public:
    i2c(TwoWire& wire = Wire) : _wire(&wire) {} // Constructor
    ~i2c() {}

    int readRegister(uint8_t slaveAddress, uint8_t address);
    int readRegisters(uint8_t slaveAddress, uint8_t address, uint8_t* data, size_t length);
    int writeRegister(uint8_t slaveAddress, uint8_t address, uint8_t value);

    TwoWire* _wire;
};

#endif // I2C_GENERIC_H
