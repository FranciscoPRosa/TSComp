#include "Accelerometer_generic.h"

#define ACCEL_ADDRESS      0x6b
#define WHO_AM_I           0x0f
#define STATUS_REG         0x17
#define CTRL_REG6_XL       0x20
#define CTRL_REG8          0x22
#define OUT_X_XL           0x28
#define CTRL_REG1_G        0x10

Q16_16 k = Q16_16::fromFloat(1.0 / 32768.0);
float offset = 0.0;

Accelerometer::Accelerometer(i2c& i2cObj) :
    continuousMode(false), _i2c(&i2cObj) // Initialize the i2c pointer
{
}

Accelerometer::~Accelerometer()
{
}

int Accelerometer::begin()
{
    _i2c->_wire->begin();
    //_i2c->begin();
    // Reset
    writeRegister(ACCEL_ADDRESS, CTRL_REG8, 0x05);
    delay(10);

    if (readRegister(ACCEL_ADDRESS, WHO_AM_I) != 0x68) {
        end();
        return 0;
    }

    writeRegister(ACCEL_ADDRESS, CTRL_REG1_G, 0x78); // 119 Hz, 2000 dps, 16 Hz BW
    writeRegister(ACCEL_ADDRESS, CTRL_REG6_XL, 0x70); // 119 Hz, 4g

    offset = getAngle(offset);

    return 1;
}

void Accelerometer::end()
{
    writeRegister(ACCEL_ADDRESS, CTRL_REG1_G, 0x00);
    writeRegister(ACCEL_ADDRESS, CTRL_REG6_XL, 0x00);
    _i2c->_wire->end();

   // _i2c->end();
}

void Accelerometer::setContinuousMode()
{
    // Enable FIFO
    writeRegister(ACCEL_ADDRESS, 0x23, 0x02);
    // Set continuous mode
    writeRegister(ACCEL_ADDRESS, 0x2E, 0xC0);
    continuousMode = true;
}

float Accelerometer::getAngle(float& angle)
{
    int16_t data[3];
    Q16_16 x, y, z;

    Q16_16 qX = Q16_16::fromInt(0), qY = Q16_16::fromInt(0), qZ = Q16_16::fromInt(0);

    for(int i = 0; i < 4; i++){
        if (!readRegisters(ACCEL_ADDRESS, OUT_X_XL, (uint8_t*)data, sizeof(data))) {
            x = NAN;
            y = NAN;
            z = NAN;
            return 0;
        }
        qX = qX + Q16_16::fromInt(data[0]);
        qY = qY + Q16_16::fromInt(data[1]);
        qZ = qZ + Q16_16::fromInt(data[2]);
    }

    x = qX * k;
    y = qY * k;
    z = qZ * k;
    return calculateRoll(x.toFloat()/4, y.toFloat()/4, z.toFloat()/4)-offset; // Convert from radians to degrees
}

int Accelerometer::accelerationAvailable()
{
    if (continuousMode) {
        // Read FIFO_SRC
        if (readRegister(ACCEL_ADDRESS, 0x2F) & 63) {
            return 1;
        }
    } else {
        if (readRegister(ACCEL_ADDRESS, STATUS_REG) & 0x01) {
            return 1;
        }
    }
    return 0;
}

// Private helper functions
float Accelerometer::calculateRoll(float x, float y, float z)
{
    return atan2(y, sqrt(x * x + z * z)) * 180 / PI;
}

int Accelerometer::readRegister(uint8_t slaveAddress, uint8_t address)
{
    return _i2c->readRegister(slaveAddress, address);
}

int Accelerometer::readRegisters(uint8_t slaveAddress, uint8_t address, uint8_t* data, size_t length)
{
    return _i2c->readRegisters(slaveAddress, address, data, length);
}

int Accelerometer::writeRegister(uint8_t slaveAddress, uint8_t address, uint8_t value)
{
    return _i2c->writeRegister(slaveAddress, address, value);
}
