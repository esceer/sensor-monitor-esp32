#ifndef Ewifi_h
#define Ewifi_h

#include <WiFi.h>

class Ewifi {
  private:
    const char* ssid;
    const char* password;

  public:
  	Ewifi(const char* wifiSsid, const char* wifiPassword);
  	void connect();
  	boolean isConnected();
};
#endif