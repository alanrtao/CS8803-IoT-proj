
#include "helpers.h"
#include "veml.h"
#include "tcs.h"

int ledMode = 0;

unsigned long discoverLoopLast = 0;
unsigned long discoverLoopLast_b = 0;

void setup() {
  Wire.begin();
  I2C_B.begin(25, 26, 100000);
  
  
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  discoverLoopLast = millis();
  discoverLoopLast_b = millis();
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


void ctlLoop2() {
  
  if(millis() - discoverLoopLast_b < 1000) return;
  discoverLoopLast_b = millis();
  Serial.println("Ctrl loop B");
  
  if (!tcs_b_began) {  
    Serial.println("start TCS[B]");
    tcs_a_began = tcs_a.begin(TCS34725_ADDRESS, &I2C_B);  
    if (!tcs_b_began) {
      Serial.println("start TCS[B] failed!");
    }
  }

  // refactor to support another i2c bus
  if (i2c_exists_b(tcs_addr)) {
    if (!tcs_b_began) {  
      tcs_b_began = tcs_b.begin(tcs_addr, &I2C_B);  
      if (!tcs_b_began) {
        Serial.println("start TCS[B] failed!");
        goto tcs_done;
      }
    }

    tcs_b_loop();
  }
  else {
    Serial.println("TCS[B] not on I2C[B]");
  }
  
  tcs_done:
  // refactor to support another i2c bus
  if (i2c_exists_b(veml_addr)) {
    if (!veml_b_began) {  
      veml_b_began = veml_b.begin(&I2C_B);  
      if (!veml_b_began) {
        Serial.println("start VEML[B] failed!");
        goto veml_done;
      }
    }

    veml_b_loop();
  }
  else {
    Serial.println("VEML[B] not on I2C[B]");
  }

  veml_done:
  return;
}

void ledLoop() {
  unsigned long c1 = millis() % (255 * 2);
  unsigned long c2 = millis() % (255);
  analogWrite(LED_BUILTIN, c1 != c2 ? (255 * 2) - c1 : c2);
}

void loop() {
  ctlLoop();  
  ctlLoop2();  
  ledLoop();  
}
