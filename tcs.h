#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs_a = Adafruit_TCS34725();
Adafruit_TCS34725 tcs_b = Adafruit_TCS34725();

#define tcs_addr 0x29

bool tcs_a_began = false;

void tcs_a_loop() {
  uint16_t r, g, b, c, colorTemp, lux;
  
  tcs_a.enable();
  tcs_a.clearInterrupt();

  tcs_a.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs_a.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs_a.calculateLux(r, g, b);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
}

void tcs_b_loop() {
  
}
