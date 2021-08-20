#ifndef EdgeML_h
#define EdgeML_h

#include "Arduino.h"
#include "IncrementalRecorder.h"
#include "HTTPClient.h"

class Recorder {
  public:
    Recorder(String backendUrl, String deviceApiKey);
    IncrementalRecorder* getIncrementalRecorder(String datasetName);
    ~Recorder();

  private:
    unsigned long long getTime();
    String _backendUrl;
    String _deviceApiKey;
    String INITDATASETINCREMENT = "/api/deviceapi/initDatasetIncrement/";
};

#endif
