#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725();

#define tcs_addr 0x29

bool tcs_began = false;

void tcs_loop() {
  uint16_t r, g, b, c;

  // https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf
  tcs.setGain(TCS34725_GAIN_16X);
  tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_300MS);
  delay(300); // Delay for one new integ. time period (to allow new reading)
  tcs.getRawData(&r, &g, &b, &c);
  
  // Create JSON object
  StaticJsonDocument<200> jsonDoc;
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

  Serial.print("[TCS]"); // Serial.print(colorTemp, DEC); Serial.print(" K - ");
  // Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
}