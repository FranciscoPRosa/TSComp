#include <Arduino.h>
#include <Wire.h>

class i2c {
private:
    TwoWire* _wire;

public:
    i2c(TwoWire& wire) : _wire(&wire) {}  // Constructor to initialize _wire

    int readRegister(uint8_t slaveAddress, uint8_t address);
    int readRegisters(uint8_t slaveAddress, uint8_t address, uint8_t* data, size_t length);
    int writeRegister(uint8_t slaveAddress, uint8_t address, uint8_t value);
};
