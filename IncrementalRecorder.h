#ifndef IncrementalRecorder_h
#define IncrementalRecroder_h

#include "Arduino.h"
#include "HTTPClient.h"
#include "Arduino_JSON.h"

class IncrementalRecorder {
  public:
    IncrementalRecorder(String backendUrl, String projectKey, String datasetName, String datasetKey, unsigned long long calcTime, bool useServertime);
    bool addDataPoint(String sensorName, double dataPoint);

  private:
    String _backendUrl;
    String _datasetKey;
    String _projectKey;
    String _datasetName;
    bool _useServerTime;
    unsigned long long _calcTime;
    String ADDDATASETINCREMENT = "/api/deviceapi/addDatasetIncrement";
};

#endif
