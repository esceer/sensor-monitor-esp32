#ifndef Emqtt_h
#define Emqtt_h

#include <WiFi.h>
#include <PubSubClient.h>

class Emqtt {
  private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    const char* clientId;
    const char* topic;

  public:
    static const char* dataPackSeparator;
    static const char* keyValueSeparator;

    Emqtt(const char* mqttBrokerIp, int mqttBrokerPort, const char* mqttTopic, const char* clientIdentifier);
    void connect();
    boolean reconnect();
    boolean publish(const char* message);
};
#endif