#include "Q16_16.h"

// Default constructor
Q16_16::Q16_16() : value(0) {}

// Constructor from integer value
Q16_16::Q16_16(int32_t v) : value(v) {}

// Convert integer to Q16.16 format
Q16_16 Q16_16::fromInt(int v) {
    return Q16_16(v * 65536);
}

// Convert float to Q16.16 format
Q16_16 Q16_16::fromFloat(float v) {
    return Q16_16(int(v * 65536));
}

// Convert Q16.16 to integer format (divide by 2^16)
int Q16_16::toInt() const {
    return value / 65536;
}

// Add two Q16.16 numbers
Q16_16 Q16_16::operator+(const Q16_16& other) const {
    return Q16_16(value + other.value);
}

// Subtract two Q16.16 numbers
Q16_16 Q16_16::operator-(const Q16_16& other) const {
    return Q16_16(value - other.value);
}

// Multiply two Q16.16 numbers and truncate to Q16.16
Q16_16 Q16_16::operator*(const Q16_16& other) const {
    // Result is a Q32.32, truncate to Q16.16
    int64_t result = (int64_t)value * other.value;
    return Q16_16((int32_t)(result >> 16)); // Truncate by shifting 16 bits
}

// Divide two Q16.16 numbers and truncate to Q16.16
Q16_16 Q16_16::operator/(const Q16_16& other) const {
    // Avoid division by zero
    if (other.value == 0) {
        return Q16_16(0); // Return 0 in case of division by zero
    }
    // Result is a Q32.32, truncate to Q16.16
    int64_t result = ((int64_t)value << 16) / other.value;
    return Q16_16((int32_t)result);
}

// Output the Q16.16 value as a float for easy debugging
float Q16_16::toFloat() const {
    return (float)value / 65536.0;
}
