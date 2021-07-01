#ifndef Recorder_h
#define Recorder_h

#include "Arduino.h"
#include "IncrementalRecorder.h"
#include "HTTPClient.h"
#include "Arduino_JSON.h"

class Recorder {
  public:
    Recorder(String backendUrl, String deviceApiKey);
    IncrementalRecorder* getIncrementalRecorder(String datasetName, bool useServerTime);

  private:
    unsigned long long getTime();
    String _backendUrl;
    String _deviceApiKey;
    HTTPClient _httpClient;
    String INITDATASETINCREMENT = "/api/deviceapi/initDatasetIncrement/";
};

#endif
