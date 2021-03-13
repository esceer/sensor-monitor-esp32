#ifndef EthSensor_h
#define EthSensor_h

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define CONVERTER_BUFFER_SIZE 10
#define SEALEVELPRESSURE_HPA (1013.25)

class EthSensor {
  private:
    Adafruit_BME280 bme;

  public:
    EthSensor();
    const char* getTemperatureValue();
    const char* getHumidityValue();
    void connect();
};
#endif