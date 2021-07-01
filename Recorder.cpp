#include "Arduino.h"
#include "WiFi.h"
#include "Recorder.h"
#include "HttpClient.h"
#include "Arduino_JSON.h"

Recorder::Recorder(String backendUrl, String deviceApiKey) {
  _backendUrl = backendUrl;
  _deviceApiKey = deviceApiKey;
}

IncrementalRecorder* Recorder::getIncrementalRecorder(String datasetName, bool useServerTime){
  unsigned long long  currentTime;
  unsigned long long calcTime;
  if (!useServerTime) {
    currentTime = getTime();
    if (currentTime == 0) {
     Serial.println("Error getting time");
    }
    calcTime = (unsigned long long) currentTime - millis();
  }
  HTTPClient http; 
  String reqAddr = _backendUrl + INITDATASETINCREMENT;
  http.begin(reqAddr.c_str());  
  http.addHeader("Content-Type", "application/json");
  JSONVar reqObj;
  reqObj["deviceApiKey"] = _deviceApiKey;
  reqObj["name"] = datasetName;
  int resCode = http.POST(JSON.stringify(reqObj));
  String res = http.getString();
  JSONVar resObj = JSON.parse(res);
  const char* datasetKey = (const char*) resObj["datasetKey"];
  
  IncrementalRecorder* incRec = new IncrementalRecorder(_backendUrl, _deviceApiKey, datasetName, datasetKey, calcTime, useServerTime);
  return incRec;
}

unsigned long long Recorder::getTime() {
  configTime(0, 0, "pool.ntp.org");
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return (unsigned long long) now * 1000;
}
