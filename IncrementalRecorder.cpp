#define ARDUINOJSON_USE_LONG_LONG 1
#include "IncrementalRecorder.h"

IncrementalRecorder::IncrementalRecorder(String backendUrl, String projectKey, String datasetName, String datasetKey, unsigned long long calcTime){
  _backendUrl = backendUrl;
  _projectKey = projectKey;
  _datasetName = datasetName;
  _datasetKey = datasetKey;
  _calcTime = calcTime;
  ctr = 0;
  uploadCounter = 0;
  datapoints = new LinkedList<DATAPOINT>();
}

void IncrementalRecorder::uploadData(void *datapoints) {
  LinkedList<DATAPOINT> *tmpList = (LinkedList<DATAPOINT>*) datapoints;
  http.begin((_backendUrl + ADDDATASETINCREMENT).c_str());
  http.addHeader("Content-Type", "application/json");
  String jsonString;

  DynamicJsonDocument doc(16384);
  doc["datasetKey"] = _datasetKey;
  JsonArray arr = doc.createNestedArray("data");
  
  for (int i = 0; i < tmpList->size(); i++) {
    char strTime[16];
    sprintf(strTime, "%lld", tmpList->get(i).time);
    String concatData;
    concatData.concat(strTime);
    concatData.concat(";");
    concatData.concat(tmpList->get(i).value);
    concatData.concat(";");
    concatData.concat(tmpList->get(i).sensorName);
    arr.add(concatData);
    concatData.~String();
  }
  serializeJsonPretty(doc, jsonString);
  int resCode = http.POST(jsonString);
  if (resCode < 0) {
    Serial.println(http.errorToString(resCode).c_str());
  }
  if (!(resCode >= 200 && resCode < 300)) {
    Serial.print("Sending data failed: ");
    Serial.println(resCode);
  } 
  doc.~BasicJsonDocument();
  tmpList->~LinkedList();
  jsonString.~String();
  uploadCounter--;
  vTaskDelete( NULL );
}

void IncrementalRecorder::addDataPoint(unsigned long long dataPointTime, const char* sensorName, double dataPoint) {
  datapoints->add(DATAPOINT{dataPoint, dataPointTime, sensorName});
  ctr++;
  if (ctr > 200) {
    uploadCounter++;
    xTaskCreate(uploadData, "upload", 10000, datapoints, 2, NULL);
    datapoints = new LinkedList<DATAPOINT>();
    ctr = 0;
  }
}

void IncrementalRecorder::addDataPoint(const char* sensorName, double dataPoint) {
  unsigned long long dataPointTime = _calcTime + millis();
  addDataPoint(dataPointTime, sensorName, dataPoint);
}

void IncrementalRecorder::onComplete() {
  uploadCounter++;
  Serial.print("UploadCounter onComplete: ");
  xTaskCreate(uploadData, "upload", 10000, datapoints, 2, NULL);
  while(uploadCounter != 0) {
    delay(10);
  }
  uploadCounter = 0;
}

IncrementalRecorder::~IncrementalRecorder() {
  datapoints->~LinkedList();
  _projectKey.~String();
  _datasetKey.~String();
  http.~HTTPClient();
}
