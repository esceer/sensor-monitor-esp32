#include <Arduino.h>
#include "Emqtt.h"

const char* Emqtt::dataPackSeparator = "#";
const char* Emqtt::keyValueSeparator = ":";

Emqtt::Emqtt(const char* mqttBrokerIp, int mqttBrokerPort, const char* mqttTopic, const char* clientIdentifier) {
  wifiClient = WiFiClient();
  mqttClient = PubSubClient(mqttBrokerIp, mqttBrokerPort, wifiClient);
  clientId = clientIdentifier;
  topic = mqttTopic;
}

void Emqtt::connect() {
  if (!reconnect()) {
    Serial.println("Failed to connect to mqtt broker");
    abort();
  }
}

boolean Emqtt::reconnect() {
  int retryCount = 12;
  while (!mqttClient.connected() && retryCount-- > 0) {
    Serial.print("Attempting MQTT connection");
    if (mqttClient.connect(clientId)) {
      Serial.println("");
      Serial.println("Connected to MQTT broker");
      return true;
    } else {
      Serial.println("...");
      delay(5000);
    }
  }
  return false;
}

boolean Emqtt::publish(const char* message) {
  if (!mqttClient.connected()) {
    if (!reconnect()) {
      return false;
    }
  }
  mqttClient.publish(topic, message);
  return true;
}