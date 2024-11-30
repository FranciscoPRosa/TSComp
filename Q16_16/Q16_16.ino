#include "Q16_16.h"

Q16_16 a = Q16_16::fromFloat(2);  // Convert integer to Q16.16 (5 -> 5 * 65536)
Q16_16 b = Q16_16::fromInt(3);  // Convert integer to Q16.16 (3 -> 3 * 65536)

Q16_16 pi = Q16_16::fromFloat(3.14159);  // Convert float to Q16.16 (3.14159 -> 205887)

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Perform operations and display results
  Q16_16 result;

  // Multiplication
  result = a * b;
  Serial.print("Result of multiplication: ");
  Serial.println(result.toFloat(), 6);  // Print result as float with 6 decimal precision

  // Addition
  result = a + b;
  Serial.print("Result of addition: ");
  Serial.println(result.toFloat(), 6);

  // Subtraction
  result = a - b;
  Serial.print("Result of subtraction: ");
  Serial.println(result.toFloat(), 6);

  // Division
  result = a / b;
  Serial.print("Result of division: ");
  Serial.println(result.toFloat(), 6);
}

void loop() {
  Q16_16 result = Q16_16::fromInt(0);
  for(int i = 0; i<100; i++){
    result = result + a;
  }
  result = result - pi;
  Serial.print("Result of multiplication with pi: ");
  Serial.println(result.toFloat(), 6);
  delay(1000);
}
