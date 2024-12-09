#ifndef I2C_GENERIC_H
#define I2C_GENERIC_H

#include <Wire.h>

class i2c {
public:

    /**
     * @brief Constructor of the i2c instance
     * 
     * @param A Wire.h instance
     * 
     * @warning The Wire instance must be different according to the hardware  
     */
    i2c(TwoWire& wire = Wire) : _wire(&wire) {} // Constructor
    ~i2c() {}

    /**
     * @brief Reads a value from the slaveAddress writing in the Address 
     * 
     * @param slaveAddress, address 
     * 
     * @return Integer read from slaveAddress  
     */
    int readRegister(uint8_t slaveAddress, uint8_t address);

    /**
     * @brief Reads length values from the slaveAddress writing in the Address into the data vector
     * 
     * @param slaveAddress, address, data, length
     * 
     * @return 1 if successful, 0 otherwise  
     */
    int readRegisters(uint8_t slaveAddress, uint8_t address, uint8_t* data, size_t length);
    
    /**
     * @brief Writes value 
     * 
     * @param slaveAddress, address 
     * 
     * @return Integer read from slaveAddress  
     */
    int writeRegister(uint8_t slaveAddress, uint8_t address, uint8_t value);

    TwoWire* _wire;
};

#endif // I2C_GENERIC_H
