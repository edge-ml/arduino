# explorer-arduino
Data Collection tool for https://github.com/edge-ml. Allows to incrementally upload sensor-data to edge-ml.

## How to use

### Install manually
1. Download the latest version as a zip-file from Github releases
2. In Arduino-IDE, go to Sketch, Include Library, Add .ZIP Library... and add the downloaded zip-file.
3. Install the following required libraries using Arduino Library Manager: 
    * ArduinoJson by Benoit Blanchon
    * LinkedList by Ivan Seidel

### Install from Arduino Library Manager
1. Search "EdgeML"
2. Install the latests version

### Upload datasets in increments
```c++
#include <Arduino.h>

// Contains everything you need
#include <EdgeML.h>

// WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password =  "YOUR_PASSWORD";

// Connect a potentiometer to pin 35 of the Esp32.
const int input = 35;
int potValue = 0;

// Collect 10 samples with 50ms between the samples.
int iterDelay = 50;
int iterCtr = 10;

IncrementalRecorder *incRec;

void setup()
{
  // Connect to WiFi
  Serial.begin(115200);
  while(!Serial);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  // Generate a recorder. The values for BACKEND_URL 
  // and DEVICE_API_KEY can be obtained from the settings 
  // page of the explorer.
  Recorder *rec = new Recorder("BACKEND_URL", "DEVICE_API_KEY");
  
  // This generates a dataset with the name DATASET_NAME. 
  // You now can add data to this dataset.
  incRec = rec->getIncrementalRecorder("DATASET_NAME");
  
  Serial.println("Now uploading");
  for (int i = 0; i < iterCtr; i++) {
    unsigned long startTime = millis();
    
    // Get the current value of the potentiometer.
    potValue = analogRead(input);
    
    // Add the obtained value to the dataset.
    incRec->addDataPoint("poti", potValue);
    
    unsigned long endTime = millis();
    delay(iterDelay - (endTime - startTime));
  }

  // Needs to be called at the end of the data recording session.
  // Uploads all remaining datapoints to the server.
  // Not calling this may lead to data loss.
  incRec->onComplete();
  
  Serial.println("Finished");
}

void loop()
{
  // Don't need the loop in this example.
}
```
