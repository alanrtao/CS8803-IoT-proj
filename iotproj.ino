
#include "helpers.h"
#include "veml.h"
#include "tcs.h"

int ledMode = 0;

unsigned long discoverLoopLast = 0;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  discoverLoopLast = millis();
}

void ctlLoop() {
  
  if(millis() - discoverLoopLast < 1000) return;
  discoverLoopLast = millis();
  Serial.println("Ctrl loop");
  
  if (!tcs_a_began) {  
    Serial.println("start TCS[A]");
    tcs_a_began = tcs_a.begin();  
    if (!tcs_a_began) {
      Serial.println("start TCS[A] failed!");
    }
  }

  
  if (!veml_a_began) {  
    Serial.println("start VEML[A]");
    veml_a_began = veml_a.begin();  
    if (!veml_a_began) {
      Serial.println("start VEML[A] failed!");
    }
  }

  // refactor to support another i2c bus
  if (veml_a_began && i2c_exists(veml_addr)) veml_a_loop();
  if (tcs_a_began && i2c_exists(tcs_addr)) tcs_a_loop();
}

void ledLoop() {
  unsigned long c1 = millis() % (255 * 2);
  unsigned long c2 = millis() % (255);
  analogWrite(LED_BUILTIN, c1 != c2 ? (255 * 2) - c1 : c2);
}

void loop() {
  ctlLoop();  
  ledLoop();  
}
