// #include <Adafruit_VEML7700.h>

// Adafruit_VEML7700 veml = Adafruit_VEML7700();

// float lux = 0;

// #define veml_addr 0x10

// bool veml_began = false;

// void veml_loop() {
//   // see: https://github.com/adafruit/Adafruit_VEML7700/blob/master/Adafruit_VEML7700.h
//   // to read lux using automatic method, specify VEML_LUX_AUTO
//     veml.enable(true);
//     veml.interruptEnable(false);
//     veml.powerSaveEnable(false);
  
//     lux = veml.readLux(VEML_LUX_CORRECTED_NOWAIT);
//     Serial.print("Lux = "); Serial.println(lux);
// }


#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Adafruit_VEML7700.h>

Adafruit_VEML7700 veml = Adafruit_VEML7700();

float lux = 0;

#define veml_addr 0x10

bool veml_began = false;

void veml_loop() {
  // Initialize the sensor if not already done
  if (!veml_began) {
    if (veml.begin()) {
      veml_began = true;
    } else {
      // Handle sensor initialization failure
      // You can add error handling code here
      return;
    }
  }

  // Configure the VEML7700 sensor
  veml.enable(true);
  veml.interruptEnable(false);
  veml.powerSaveEnable(false);
  
  // Read the lux value
  lux = veml.readLux(VEML_LUX_CORRECTED_NOWAIT);
  
  // Create a JSON object and encode the lux value
  StaticJsonDocument<100> jsonDoc;
  jsonDoc["lux"] = lux;

  // Serialize the JSON object to a string
  String jsonData;
  serializeJson(jsonDoc, jsonData);

  // Send the JSON data via HTTP POST
  HTTPClient http;
  String serverUrl = "http://192.168.51.2"; // Adjust if a different port or path is needed

  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    // Optional: Handle the response from the server
    String response = http.getString();
    // You can process the response if needed
  } else {
    // Optional: Handle the error
    // You can add error handling code here
  }

  http.end(); // Free resources

  // Optional: Add a delay if needed
  // delay(1000);
}