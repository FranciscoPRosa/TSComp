#include <Arduino.h>
#include "fixedPoint.h"

void setup() {
    Serial.begin(9600);
}

void loop() {
  // Initialize the fixed point class
    intFP a(3.14159265359); // 3.1416
    intFP toRadian(0.0174532925);
    intFP angle(30.0);

    /*
    a *= (int32_t)2;
    a /= (int32_t)2;
    Serial.print("Result: "); // 6.2832
    Serial.println(a.toFloat(), 6);
    Serial.print("Real: ");
    Serial.println(3.14159265359, 6);
    */
    intFP c = toRadian * angle;
    Serial.println(c.toFloat(), 4);
    delay(5000);
}
