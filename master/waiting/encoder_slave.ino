#include <Wire.h>

void setup() {
  Wire.begin(8);
  Wire.onRequest(sendEncoder)
}

void sendEncoder() {
  Wire.write(encoderArray);
}
