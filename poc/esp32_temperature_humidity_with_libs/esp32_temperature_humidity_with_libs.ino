#include <Ewifi.h>
#include <Emqtt.h>
#include <EthSensor.h>

#define BUFFER_SIZE 50

Ewifi ewifi("<SSID>", "<PASSWORD>");
Emqtt emqtt("<IP_ADDRESS>", 1883, "ehome/sensors", "portable_box");
EthSensor sensor;

const char* temperatureSensorName = "portable_box_temperature";
const char* humiditySensorName = "portable_box_humidity";

// Timer 
unsigned int loopInterval = 5000;


void setup() {
  Serial.begin(115200);
  sensor.connect();
  ewifi.connect();
  emqtt.connect();
}

void loop() {
  if(!ewifi.isConnected()){
    Serial.println("WiFi is disconnected");
    ewifi.connect();
  }
  sendSensorData();
  delay(loopInterval);
}

void sendSensorData() {
  const char* temperatureValue = sensor.getTemperatureValue();
  const char* humidityValue = sensor.getHumidityValue();
  
  static char msg[BUFFER_SIZE];
  strcpy(msg, temperatureSensorName);
  strcat(msg, Emqtt::keyValueSeparator);
  strcat(msg, temperatureValue);
  strcat(msg, Emqtt::dataPackSeparator);
  strcat(msg, humiditySensorName);
  strcat(msg, Emqtt::keyValueSeparator);
  strcat(msg, humidityValue);

  emqtt.publish(msg);
}
