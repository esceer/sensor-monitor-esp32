#include <Arduino.h>
#include "EthSensor.h"

EthSensor::EthSensor() {
  bme = Adafruit_BME280();
}

const char* EthSensor::getTemperatureValue() {
  float temperature = bme.readTemperature();
  static char temperatureStrValue[CONVERTER_BUFFER_SIZE];
  dtostrf(temperature, 4, 2, temperatureStrValue);
  return temperatureStrValue;
}

const char* EthSensor::getHumidityValue() {
  float humidity = bme.readHumidity();
  static char humidityStrValue[CONVERTER_BUFFER_SIZE];
  dtostrf(humidity, 4, 2, humidityStrValue);
  return humidityStrValue;
}

void EthSensor::connect() {
  bme.begin(0x76);
}