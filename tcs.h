// #include "Adafruit_TCS34725.h"

// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

// #define tcs_addr 0x29

// bool tcs_began = false;

// void tcs_loop() {
//   uint16_t r, g, b, c, colorTemp, lux;
  
//   tcs.enable();
//   tcs.clearInterrupt();

//   tcs.getRawData(&r, &g, &b, &c);
//   // colorTemp = tcs.calculateColorTemperature(r, g, b);
//   colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
//   lux = tcs.calculateLux(r, g, b);

//   Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
//   Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
//   Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
//   Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
//   Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
//   Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
//   Serial.println(" ");
// }

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "Adafruit_TCS34725.h"

// Assuming Wi-Fi is already connected elsewhere in your code

Adafruit_TCS34725 tcs = Adafruit_TCS34725();

#define tcs_addr 0x29

bool tcs_began = false;

void tcs_loop() {
  uint16_t r, g, b, c;
  uint16_t colorTemp, lux;
  
  tcs.enable();
  tcs.clearInterrupt();

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  // Create JSON object
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["ColorTemp"] = colorTemp;
  jsonDoc["Lux"] = lux;
  jsonDoc["R"] = r;
  jsonDoc["G"] = g;
  jsonDoc["B"] = b;
  jsonDoc["C"] = c;

  // Serialize JSON to string
  String jsonData;
  serializeJson(jsonDoc, jsonData);

  // Send JSON data via HTTP POST
  HTTPClient http;
  String serverUrl = "http://192.168.51.2"; // Modify if a different port or path is needed

  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    // Optionally handle the response from the server
    // For example, you can print it to the Serial Monitor if needed
    // Serial.println("Response from server:");
    // Serial.println(response);
  } else {
    // Handle error
    // Serial.print("Error on sending POST: ");
    // Serial.println(httpResponseCode);
  }

  http.end(); // Free resources

  // Optionally, add a delay if needed
  // delay(1000);
}