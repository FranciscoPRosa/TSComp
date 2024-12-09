#ifndef Q16_16_H
#define Q16_16_H

#include <cstdint>  // For int32_t

class Q16_16 {
public:
    int32_t value;  // Store Q16.16 value as a 32-bit signed integer

    Q16_16();  // Default constructor
    Q16_16(int32_t v);  // Constructor from integer value

    static Q16_16 fromInt(int v);  // Convert integer to Q16.16 format
    static Q16_16 fromFloat(float v);  // Convert float to Q16.16 format
    int toInt() const;  // Convert Q16.16 to integer

    Q16_16 operator+(const Q16_16& other) const;  // Addition
    Q16_16 operator-(const Q16_16& other) const;  // Subtraction
    Q16_16 operator*(const Q16_16& other) const;  // Multiplication
    Q16_16 operator/(const Q16_16& other) const;  // Division

    float toFloat() const;  // Output as float for debugging
};

#endif
