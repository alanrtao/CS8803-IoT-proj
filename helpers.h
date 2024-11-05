
#include <Wire.h>

// https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/
bool i2c_exists(byte address) {
  byte error;
  
  Serial.println("Scan i2c A");
  Wire.beginTransmission(address);
  error = Wire.endTransmission();
  if (error == 0) {
    return true;
  }
  else if (error==4) {
    Serial.print("I2C[A] Unknow error at address 0x");
    if (address<16) {
      Serial.print("0");
    }
    
    Serial.println(address,HEX);
  }  
  return false;
}

TwoWire I2C_B = TwoWire(0);

// https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/
bool i2c_exists_b(byte address) {
  byte error;
  
  Serial.println("Scan i2c B");
  I2C_B.beginTransmission(address);
  error = I2C_B.endTransmission();
  if (error == 0) {
    return true;
  }
  else if (error==4) {
    Serial.print("I2C[B] Unknow error at address 0x");
    if (address<16) {
      Serial.print("0");
    }
    
    Serial.println(address,HEX);
  }  
  return false;
}
