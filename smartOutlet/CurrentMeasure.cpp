#include "CurrentMeasure.h"
#include "Network.h"

int CurrentMeasure::maxCurrent = -1;
float CurrentMeasure::sumCurrent = 0.0;
int CurrentMeasure::i = 0;
Mqtt CurrentMeasure::_mqtt;

CurrentMeasure::CurrentMeasure(Mqtt mqtt){
  _mqtt = mqtt;
}

void CurrentMeasure::timerLastWill(){
  Serial.println("This is Timer last will!");
}

int CurrentMeasure::getCurrentTest(){
  return 312;
}

void CurrentMeasure::readCurrent(void *context){
  int current = getCurrentTest();
  if(current > maxCurrent){
    maxCurrent = current;
  }
  
}

void CurrentMeasure::addCurrent(void *context){
  i++;
  float amplitude_current = (float)(maxCurrent)/1024*5/185*1000000;
  float effective_value = amplitude_current/1.414;
  sumCurrent = sumCurrent + 10;
  maxCurrent = -1;
}

void CurrentMeasure::sendAverageCurrent(void *context){
  Serial.print("i = ");
  Serial.println(i);
  
  String tempBuff;
  Serial.print("sumCurrent = ");
  Serial.println(sumCurrent);
  tempBuff=String(sumCurrent/i);
  Serial.print("tempBuff = ");
  Serial.println(tempBuff);
  char stringMessage[tempBuff.length()];
  tempBuff.toCharArray(stringMessage,tempBuff.length());
  sumCurrent = 0.0;
  i = 0;
  
  char topic[8 + strlen(Network::_ssidAccessPoint)];
  strcpy(topic, "sockets/");
  strcat(topic, Network::_ssidAccessPoint);
  
  _mqtt.publish(topic, stringMessage); 
  Serial.print("i = ");
  Serial.println(i);
}

void CurrentMeasure::startTimers(){
  _mqtt.connect();
  timerMilisecId = timer.every(100, readCurrent, (void*)0);
  timerSecId = timer.every(1000, addCurrent, (void*)0);
  timerMinId = timer.every(60000, sendAverageCurrent, (void*)0);
}

void CurrentMeasure::stopTimers(){
  timer.stop(timerMilisecId);
  timer.stop(timerSecId);
  timer.stop(timerMinId);
  addCurrent((void*)0);
  sendAverageCurrent((void*)0);
  timerLastWill();
  _mqtt.disconnect();
}

void CurrentMeasure::update(){
  timer.update();
}

