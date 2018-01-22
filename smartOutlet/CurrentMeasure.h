#ifndef CurrentMeasure_h
#define CurrentMeasure_h

#include <Arduino.h>
#include "Mqtt.h"
#include <Filters.h>

class CurrentMeasure{

  private:
  Mqtt mqtt;
  RunningStatistics inputStats;
  
  static const float windowLength;
  static const float intercept;
  static const float slope;
  static const float averageOfZeroAmps;

  unsigned long analogReadStartTime = 0;
  unsigned long analogReadPeriod = 4;

  static const unsigned long printPeriod;
  unsigned long previousMillis = 0;

  float sigmaSum = 0;
  int sensorValue = 0;
  int secondsCounter = 0;
  int samplesCounter = 0;

  void measure();
  void calculateSigma();
  void calibrate();
  float calculateAverageCurrent();
  void sumSigma();
  void sendMeasuredData(float averageCurrent);
  void analogReadEvery(unsigned long milisecTimePeriod);

  public:
  CurrentMeasure(Mqtt mqtt);
  CurrentMeasure();
  void update();
};

#endif
