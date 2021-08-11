#include "Recorder.h"

Recorder::Recorder(String backendUrl, String deviceApiKey) {
  _backendUrl = backendUrl;
  _deviceApiKey = deviceApiKey;
}

IncrementalRecorder* Recorder::getIncrementalRecorder(String datasetName){
  unsigned long long currentTime;
  unsigned long long calcTime;
  currentTime = getTime();
  calcTime = (unsigned long long) currentTime - millis();
  HTTPClient http;
  String reqAddr = _backendUrl + INITDATASETINCREMENT;
  http.begin(reqAddr.c_str());  
  http.addHeader("Content-Type", "application/json");
  DynamicJsonDocument reqObj(1024);
  reqObj["deviceApiKey"] = _deviceApiKey;
  reqObj["name"] = datasetName;
  String sendObj;
  serializeJson(reqObj, sendObj);
  http.POST(sendObj);
  String res = http.getString();
  DynamicJsonDocument resObj(1024);
  deserializeJson(resObj, res);
  const char* datasetKey = (const char*) resObj["datasetKey"];
  IncrementalRecorder* incRec = new IncrementalRecorder(_backendUrl, _deviceApiKey, datasetName, datasetKey, calcTime);
  http.~HTTPClient();
  sendObj.~String();
  reqAddr.~String();
  res.~String();
  
  // Will lead to heap corruption error
  //reqObj.~BasicJsonDocument();
  //resObj.~BasicJsonDocument();
  
  return incRec;
}

unsigned long long Recorder::getTime() {
  configTime(0, 0, "pool.ntp.org");
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return(0);
  }
  time(&now);
  return (unsigned long long) now * 1000;
}

Recorder::~Recorder() {
    _backendUrl.~String();
    _deviceApiKey.~String();
    INITDATASETINCREMENT.~String();
}
