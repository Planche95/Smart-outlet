#include "Network.h"
#include "Mqtt.h"
#include "CurrentMeasure.h"

Network network("123456789");
Mqtt mqtt("192.168.0.103", 1883);
CurrentMeasure currentMeasure(mqtt);

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  network.configure();
  mqtt.configure();  
  currentMeasure.startTimers();
}

void loop() {
  network.update();
  mqtt.update();
  currentMeasure.update();
}
