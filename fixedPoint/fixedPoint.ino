#include <Arduino.h>
#include "fixedPoint.h"

void setup() {
    Serial.begin(9600);
    
    // Initialize the fixed point class
    intFP a(31416, 10000); // 3.1416
    intFP b((int32_t)2); // 2.0

    intFP c = a * b; // 6.2832
    Serial.print("Result: "); // 6.2832
    Serial.println(c.toFloat());
}

void loop() {}
