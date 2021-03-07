#include <Arduino.h>
#include "Ewifi.h"

Ewifi::Ewifi(const char* wifiSsid, const char* wifiPassword) {
  ssid = wifiSsid;
  password = wifiPassword;
}

void Ewifi::connect() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(!isConnected()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

boolean Ewifi::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}