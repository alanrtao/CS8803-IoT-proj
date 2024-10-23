#include <Adafruit_VEML7700.h>

Adafruit_VEML7700 veml_b = Adafruit_VEML7700();
Adafruit_VEML7700 veml_a = Adafruit_VEML7700();

float lux_a = 0;
float lux_b = 0;

#define veml_addr 0x10

bool veml_a_began = false;

void veml_a_loop() {
  // see: https://github.com/adafruit/Adafruit_VEML7700/blob/master/Adafruit_VEML7700.h
  // to read lux using automatic method, specify VEML_LUX_AUTO
    Serial.println("VEML[A]:");
    veml_a.enable(true);
    veml_a.interruptEnable(false);
    veml_a.powerSaveEnable(false);
  
    lux_a = veml_a.readLux(VEML_LUX_CORRECTED_NOWAIT);
    Serial.print("Lux[A] = "); Serial.println(lux_a);
}

void veml_b_loop() {
  // see: https://github.com/adafruit/Adafruit_VEML7700/blob/master/Adafruit_VEML7700.h
  // to read lux using automatic method, specify VEML_LUX_AUTO

    veml_b.enable(true);
    veml_b.interruptEnable(false);
    veml_b.powerSaveEnable(false);
  
    lux_b = veml_b.readLux(VEML_LUX_AUTO);
    Serial.print("Lux[B] = "); Serial.println(lux_b);
}
