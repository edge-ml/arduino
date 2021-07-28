#include <Arduino.h>
#include <Recorder.h>

const char* ssid = "YOUR_SSID";
const char* password =  "YOUR_PASSWORD";

const int input = 35;
int potValue = 0;

int iterDelay = 50;
int iterCtr = 10;

IncrementalRecorder *incRec;

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
  Recorder *rec = new Recorder("BACKEND_URL", "DEVICE_API_KEY");
  incRec = rec->getIncrementalRecorder("testNameArduino2");
  Serial.println("Now uploading");
  for (int i = 0; i < iterCtr; i++) {
    unsigned long startTime = millis();
    potValue = analogRead(input);
    incRec->addDataPoint("poti", potValue);
    unsigned long endTime = millis();
    delay(iterDelay - (endTime - startTime));
  }
  incRec->onComplete();
  Serial.println("Finished");
}

void loop()
{
}
