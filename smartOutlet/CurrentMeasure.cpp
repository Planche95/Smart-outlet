#include "CurrentMeasure.h"
#include <Filters.h>

RunningStatistics inputStats;

CurrentMeasure::CurrentMeasure(Mqtt mqtt){
  this->mqtt = mqtt;
  inputStats.setWindowSecs(windowLength);
}

CurrentMeasure::CurrentMeasure(){

}

//mqtt.publish(topic, stringMessage); 

void CurrentMeasure::update(){
  sensorValue = analogRead(A0);  // read the analog in value:
  inputStats.input(sensorValue);  // log to Stats function
  j++;
  
  if((unsigned long)(millis() - previousMillis) >= printPeriod) {
    previousMillis = millis();   // update time
    i++;  
    // display current values to the screen
    Serial.print( "\n" );
    Serial.print("Probek: ");
    Serial.println(j);
    j = 0;
    // output sigma or variation values associated with the inputValue itsel
    Serial.print( inputStats.sigma() );
    //Serial.print(" ");
    //Serial.print(sensorValue);
    if(i == 60){
      Serial.println("");
      Serial.println("----------------- 60 sec");
      i = 0;
    }
  }
}

