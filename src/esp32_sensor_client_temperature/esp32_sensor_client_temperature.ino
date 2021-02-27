#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// The below parameters are mandatory for this script and need to be provided prior to run!
const char* WIFI_SSID = "<SSID>";
const char* WIFI_PASSWORD = "<PASSWORD>";
String SENSOR_NAME = "<SENSOR_NAME>";
String SENSOR_DATA_WAREHOUSE_FQDN = "http://<HOST>:<PORT>/";

// This represents the sensor id which comes from the server
String sensorId;
// The following variables are unsigned longs because this way millis will only overflow every 50 days.
unsigned long lastRunTime = 0;
// Set timer to 10 seconds (10000)
unsigned long timerLoopInterval = 10000;

void setup() {
  Serial.begin(115200);
  connectToWifi();
  setupSensor();
}

void loop() {
  if (needsToRun()) {
    if(isWifiConnected()){
      String sensorValue = getSensorValue();
      sendSensorUpdate(sensorValue);
    }
    else {
      Serial.println("WiFi is disconnected");
    }
    lastRunTime = millis();
  }
}

void setupSensor() {
  Serial.print("Setting up sensor: ");
  Serial.println(SENSOR_NAME);

  if (!existsSensor(SENSOR_NAME)) {
    setupNewSensor(SENSOR_NAME);
  }
  sensorId = fetchSensorId(SENSOR_NAME);
  Serial.print("Setup completed, configured sensor id is: ");
  Serial.println(sensorId);
}

boolean existsSensor(String sensorName) {
  String serverPath = SENSOR_DATA_WAREHOUSE_FQDN + "sensors/name/" + sensorName;
  String sensorInfo = sendHttpGetRequest(serverPath.c_str());
  JSONVar jsonResponse = JSON.parse(sensorInfo);
  
  if (JSON.typeof(jsonResponse) == "undefined") {
    return false;
  }
  if (!jsonResponse.hasOwnProperty("id")) {
    return false;
  }
  return true;
}

String fetchSensorId(String sensorName) {
  String serverPath = SENSOR_DATA_WAREHOUSE_FQDN + "sensors/name/" + sensorName;
  String sensorInfo = sendHttpGetRequest(serverPath.c_str());
  JSONVar jsonResponse = JSON.parse(sensorInfo);
  
  if (JSON.typeof(jsonResponse) == "undefined") {
    Serial.println("Parsing fetchSensorId response failed!");
    return "fallback";
  }
  
  if (!jsonResponse.hasOwnProperty("id")) {
    Serial.println("No sensor id was received from server. Setup finished with error.");
    return "fallback";
  }
  const char* id = jsonResponse["id"];
  return String(id);
}

void setupNewSensor(String sensorName) {
  String serverPath = SENSOR_DATA_WAREHOUSE_FQDN + "sensors/" + sensorName + "/state/init";
  sendHttpPostRequest(serverPath.c_str());
}

String getSensorValue() {
  return "0.123456789";
}

void sendSensorUpdate(String sensorValue) {
  String serverPath = SENSOR_DATA_WAREHOUSE_FQDN + "sensors/" + sensorId + "/state/" + sensorValue;
  sendHttpPutRequest(serverPath.c_str());
}

String sendHttpGetRequest(const char* serverPath) {
  HTTPClient http;
  http.begin(serverPath);

  int httpResponseCode = http.GET();

  String payload = "{}"; 

  if (httpResponseCode>0) {
    payload = http.getString();
  }
  else {
    Serial.print("Error code for get request: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}

void sendHttpPostRequest(const char* serverPath) {
  HTTPClient http;
  http.begin(serverPath);
  
  int httpResponseCode = http.POST("");
  
  if (httpResponseCode<=0) {
    Serial.print("Error code for post request: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void sendHttpPutRequest(const char* serverPath) {
  HTTPClient http;
  http.begin(serverPath);
  http.addHeader("Content-Type", "text/plain"); 

  int httpResponseCode = http.PUT("ESP32");
  
  if (httpResponseCode<=0) {
    Serial.print("Error code for put request: ");
    Serial.println(httpResponseCode);
  }

  http.end();
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

boolean needsToRun() {
  return (millis() - lastRunTime) > timerLoopInterval;
}
