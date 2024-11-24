#include <Adafruit_VEML7700.h>

Adafruit_VEML7700 veml = Adafruit_VEML7700();

float lux = 0;

#define veml_addr 0x10

bool veml_began = false;

void veml_loop() {
  veml.enable(true);
  veml.interruptEnable(false);
  veml.powerSaveEnable(false);
  lux = veml.readLux(VEML_LUX_CORRECTED_NOWAIT);

  Serial.printf("[VEML] %f\n", lux);
}
