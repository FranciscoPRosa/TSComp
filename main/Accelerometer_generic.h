#ifndef ACCELEROMETER_GENERIC_H
#define ACCELEROMETER_GENERIC_H

#include "i2c_generic.h" // Include the i2c class definition
#include <Arduino.h>
#include <Wire.h>
#include "Q16_16.h"

class Accelerometer {
public:
    /**
     * @brief Construct a new Accelerometer Object. Also set countinous mode to false (single reading).
     * 
     * @param i2cObj A generic I2C instance for the communication with the LSM9DS1 Inertial Module.
     * 
     * @warning The generic instance should be Wire1 for the Arduino Nano 33 BLE model.
     */
    Accelerometer(i2c& i2cObj); // Constructor takes an i2c object
    /**
     * @brief Destructs this instance of as Accelerometer Object.
     * 
     * .
     * 
     * 
     */
    ~Accelerometer();

    /**
     * @brief Initializes the instance and the necessary registers for I2C communication. Also measures the offset of the floor. 
     * 
     * 
     * 
     * 
     * @warning For this initialization, the sensor has to completely to obtain a correct offset.
     */
    int begin();

    /**
     * @brief Ends the reading process.
     * 
     *
     * 
     * 
     */
    void end();

    /**
     * @brief Turns continous mode on
     * 
     * 
     * 
     * 
     */
    void setContinuousMode();

    /**
     * @brief Measures (average of 4 readings) and updates the xyz coordinates.
     * 
     * 
     * 
     * @return 1 if the sensor is available and the registers is read with success, 0 otherwise
     */
    bool getAngle();

    /**
     * @brief Checks if the correct value is in the registers
     * 
     * 
     * 
     * @return 1 if the status registers have the correct value, 0 otherwise
     */
    int accelerationAvailable();

    /**
     * @brief According to the last values obtained for the xyz coordinates calculates the roll (estimated inclination)
     * 
     * @warning The atan2 function consumes some resources, should be used as least as possible
     * 
     * @return The angle estimation after a reading
     */
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
