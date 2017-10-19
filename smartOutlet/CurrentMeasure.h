#ifndef CurrentMeasure_h
#define CurrentMeasure_h

#include <Arduino.h>
#include "Mqtt.h"

class CurrentMeasure{

  private:
  Mqtt mqtt;
  
  float windowLength = 20.0/60;
  int sensorValue = 0;
  float intercept = -0.1310; // to be adjusted based on calibration testing
  float slope = 0.04099; // to be adjusted based on calibration testing
  float current_amps; // actual measure current
  
  unsigned long printPeriod = 1000;
  unsigned long previousMillis = 0;

  //TODO for tests
  int i = 0;
  int j = 0;

  public:
  CurrentMeasure(Mqtt mqtt);
  CurrentMeasure();
  void update();
};

#endif
