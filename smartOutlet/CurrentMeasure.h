#ifndef CurrentMeasure_h
#define CurrentMeasure_h

#include <Arduino.h>
#include "Timer.h"
#include "Mqtt.h"

class CurrentMeasure{

  private:
  Timer timer;
  static Mqtt _mqtt;
  int timerMilisecId;
  int timerSecId;
  int timerMinId;
  
  static int maxCurrent;
  static float sumCurrent;
  static int i;

  void timerLastWill();
  static int getCurrentTest();
  static void readCurrent(void *context);
  static void addCurrent(void *context);
  static void sendAverageCurrent(void *context);

  public:
  CurrentMeasure(Mqtt mqtt);
  void startTimers();
  void stopTimers();
  void update();
};

#endif
