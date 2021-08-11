#ifndef IncrementalRecorder_h
#define IncrementalRecroder_h
#define ARDUINOJSON_USE_LONG_LONG 1
#include "Arduino.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"
#include "LinkedList.h"

class IncrementalRecorder {
  public:
    IncrementalRecorder(String backendUrl, String projectKey, String datasetName, String datasetKey, unsigned long long calcTime);
    void addDataPoint(const char* sensorName, double dataPoint);
    void addDataPoint(unsigned long long dataPointTime, const char* sensorName, double dataPoint);
    ~IncrementalRecorder();
    static void uploadData(void *req);
    void onComplete();

    struct DATAPOINT {
      double value;
      unsigned long long time;
      const char* sensorName;
    };

  private:

    LinkedList<DATAPOINT> *datapoints;
    DATAPOINT dataArray[20];
    String _projectKey;
    String _datasetName;
    bool _useDeviceTime;
    int ctr;
    unsigned long long _calcTime;

};
static String ADDDATASETINCREMENT = "/api/deviceapi/addDatasetIncrementIot/";
static String _backendUrl;
static HTTPClient http;
static int uploadCounter;
static String _datasetKey;

#endif
