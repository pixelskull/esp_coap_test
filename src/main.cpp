#include <TinyGsmClient.h>
#include <WiFiUdp.h>
#include <CoAP.h>
#include "config.h"

#define MODEM_RST         5
#define MODEM_PWKEY       4
#define MODEM_POWER_ON    23
#define MODEM_TX          27
#define MODEM_RX          26

#define COAP_PORT         5683
#define COAP_RESOURCE     "/locations"

TinyGsm modem(Serial1);
TinyGsmClient client(modem);
WiFiUDP Udp;
Coap coap(Udp);

void setup() {
  // Start Serial for debug purposes
  Serial.begin(115200);
  delay(10);

  // Set the pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);

  // Restart takes quite some time
  modem.restart();
  
  // Initialize CoAP client
  coap.start();

  // Connect to the WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
}

void loop() {
  // Mock values for IMEI, latitude, and longitude
  String imei = "123456789012345";
  String lat = "12.34567";
  String lon = "89.01234";

  // Create JSON message
  String jsonMessage = "{\"imei\":\"" + imei + "\",\"latitude\":\"" + lat + "\",\"longitude\":\"" + lon + "\"}";

  // Send the JSON message via CoAP
  coap.post(COAP_SERVER, COAP_PORT, COAP_RESOURCE, jsonMessage.c_str());

  delay(1000);  // Send message every second
}

