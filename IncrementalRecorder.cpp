#include "Arduino.h"
#include "WiFi.h"
#include "IncrementalRecorder.h"
#include "HttpClient.h"
#include "Arduino_JSON.h"

IncrementalRecorder::IncrementalRecorder(String backendUrl, String projectKey, String datasetName, String datasetKey, unsigned long long calcTime, bool useServerTime){
  _backendUrl = backendUrl;
  _projectKey = projectKey;
  _datasetName = datasetName;
  _useServerTime = useServerTime;
  _datasetKey = datasetKey;
  _calcTime = calcTime;
}

bool IncrementalRecorder::addDataPoint(String sensorName, double dataPoint) {
  Serial.println(_calcTime);
  Serial.println(millis());
  unsigned long long dataPointTime = _calcTime + millis();
  HTTPClient http; 
  String reqAddr = _backendUrl + ADDDATASETINCREMENT; 
  http.begin(reqAddr.c_str());  
  http.addHeader("Content-Type", "application/json");
  JSONVar reqObj;
  reqObj["datasetKey"] = _datasetKey; 
  reqObj["datapoint"] = dataPoint;
  reqObj["sensorname"] = sensorName;
  if (!_useServerTime) {
    Serial.print("LTime: ");
    Serial.println(dataPointTime);
    char str[256];
    sprintf(str, "%lld", dataPointTime);
    Serial.print("Time: ");
    Serial.println(str);
    reqObj["time"] = str;
  }
  int resCode = http.POST(JSON.stringify(reqObj));
  //JSONVar resObj = JSON.parse(http.getString());
  if (resCode >= 200 && resCode < 300) {
    return true;
  }
  return false;
}
