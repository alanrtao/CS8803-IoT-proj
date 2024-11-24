#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725();

#define tcs_addr 0x29

bool tcs_began = false;
uint16_t r, g, b, c;

void tcs_loop() {

  // https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf
  tcs.setGain(TCS34725_GAIN_16X);
  tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_300MS);
  delay(300); // Delay for one new integ. time period (to allow new reading)
  tcs.getRawData(&r, &g, &b, &c);

  Serial.print("[TCS]");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
}