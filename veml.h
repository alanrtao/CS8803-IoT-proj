#include <Adafruit_VEML7700.h>

Adafruit_VEML7700 veml = Adafruit_VEML7700();

float lux = 0;

#define veml_addr 0x10

bool veml_began = false;

void veml_loop() {
  // see: https://github.com/adafruit/Adafruit_VEML7700/blob/master/Adafruit_VEML7700.h
  // to read lux using automatic method, specify VEML_LUX_AUTO
    veml.enable(true);
    veml.interruptEnable(false);
    veml.powerSaveEnable(false);
  
    lux = veml.readLux(VEML_LUX_CORRECTED_NOWAIT);
    Serial.print("Lux = "); Serial.println(lux);
}
