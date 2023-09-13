#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <coap-simple.h>
// CoAP Server settings
const int coapPort = 5683; // Default CoAP port

// CoAP client instance
WiFiUDP udp;
Coap coap(udp);

uint16_t messageID = 0;

void setup() {
  // Start serial
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start CoAP client
  coap.start();
}

void loop() {
  // Create JSON with dummy values
  String jsonData =
      "{\"imei\":\"123456789\",\"latitude\":50.1109,\"longitude\":8.6821}";

  // Convert JSON string to char array
  char payload[jsonData.length() + 1];
  jsonData.toCharArray(payload, sizeof(payload));

  // Send CoAP POST request
  coap.send(IPAddress(coapServerIP), coapPort, "locations", COAP_NONCON,
            COAP_POST,
            NULL, // token
            0,    // token length
            (uint8_t *)payload, jsonData.length(), COAP_CONTENT_TYPE::COAP_NONE,
            messageID++);
  Serial.println("sended package...");

  // Wait before sending again
  delay(1000);
}
