#include <Arduino.h>
#include "EthSensor.h"

EthSensor::EthSensor() {
  bme = Adafruit_BME280();
}

const char* EthSensor::getTemperatureValue() {
  float temperature = bme.readTemperature();
  static char temperatureStrValue[CONVERTER_BUFFER_SIZE];
  return convertFloatToString(temperature, temperatureStrValue);
}

const char* EthSensor::getHumidityValue() {
  float humidity = bme.readHumidity();
  static char humidityStrValue[CONVERTER_BUFFER_SIZE];
  return convertFloatToString(humidity, humidityStrValue);
}

const char* EthSensor::convertFloatToString(float value, char output[]) {
	return dtostrf(value, 4, 2, output);
}

void EthSensor::connect() {
  bme.begin(0x76);
}