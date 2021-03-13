#include <WiFi.h>

// MQTT
#include <PubSubClient.h>

// SENSOR
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// The below parameters are mandatory for this script and need to be provided prior to run!
const char* WIFI_SSID = "<SSID>";
const char* WIFI_PASSWORD = "<PASSWORD>";

const char* MQTT_BROKER_IP = "<IP_ADDRESS>";
unsigned int MQTT_BROKER_PORT = 1883;
const char* MQTT_TOPIC = "ehome/sensors";

String SENSOR_NAME = "portable_box";
String TEMPERATURE_SENSOR_NAME = SENSOR_NAME + "_temperature";
String HUMIDITY_SENSOR_NAME = SENSOR_NAME + "_humidity";

String DATA_PACK_SEPARATOR = "#";
String KEY_VALUE_SEPARATOR = ":";

// Timer 
unsigned int loopInterval = 10000;

// MQTT
WiFiClient wifiClient;
PubSubClient mqttClient(MQTT_BROKER_IP, MQTT_BROKER_PORT, wifiClient);

// SENSOR
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C

void setup() {
  Serial.begin(9600);
  connectToWifi();
  connectToMqttBroker();
  connectToSensor();
}

void loop() {
  if(!isWifiConnected()){
    Serial.println("WiFi is disconnected");
    connectToWifi();
  }
  sendSensorUpdate();
  delay(loopInterval);
}

void sendSensorUpdate() {
  String temperatureValue = getTemperatureSensorValue();
  String humidityValue = getHumiditySensorValue();
  String msg = TEMPERATURE_SENSOR_NAME + KEY_VALUE_SEPARATOR + temperatureValue
             + DATA_PACK_SEPARATOR
             + HUMIDITY_SENSOR_NAME + KEY_VALUE_SEPARATOR + humidityValue;
  publishToMqtt(msg);
}

String getTemperatureSensorValue() {
  float temperature = bme.readTemperature();
  return convertFloatToString(temperature);
}

String getHumiditySensorValue() {
  float humidity = bme.readHumidity();
  return convertFloatToString(humidity);
}

String convertFloatToString(float value) {
  // TODO: Out of memory because of String class in the long run?
  return String(value);
}

void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while(!isWifiConnected()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

boolean isWifiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

boolean connectToMqttBroker() {
  int retryCount = 12;
  while (!mqttClient.connected() && retryCount-- > 0) {
    Serial.print("Attempting MQTT connection");
    if (mqttClient.connect(SENSOR_NAME.c_str())) {
      Serial.println("");
      Serial.println("Connected to MQTT broker");
      return true;
    } else {
      Serial.print(".");
      delay(5000);
    }
  }
  return false;
}

void publishToMqtt(String message) {
  if (!mqttClient.connected()) {
    if (!connectToMqttBroker()) {
      // Reset loop, might need to reconnect to wifi
      return;
    }
  }
  mqttClient.publish(MQTT_TOPIC, message.c_str());
}

void connectToSensor() {
  bme.begin(0x76);
}
