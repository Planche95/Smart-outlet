#include "CurrentMeasure.h"

const float CurrentMeasure::windowLength = 0.75;
const float CurrentMeasure::intercept = -0.003;
const float CurrentMeasure::slope = 0.036;
const unsigned long CurrentMeasure::printPeriod = 1000;
const float CurrentMeasure::averageOfZeroAmps = 60.0;

CurrentMeasure::CurrentMeasure(Mqtt mqtt){
  this->mqtt = mqtt;
  inputStats.setWindowSecs(windowLength);
}

CurrentMeasure::CurrentMeasure(){
  
}

//TODO clean code
int j =0;
int initialDelay = 0;

int maxSec = 0;
long analogMinSum = 0;

void CurrentMeasure::measure(){
  analogReadEvery(analogReadPeriod);
      
  if((unsigned long)(millis() - previousMillis) >= printPeriod) {
    previousMillis = millis();
    
    if(initialDelay < 4){
      initialDelay++;
      Serial.println("WAIT");
      j=0;
      analogSecSum = 0;
    } else{
      sumSigma();
      secondsCounter++;    

      //Serial.print(" sensorValue: ");
      //Serial.print(sensorValue);

      //Serial.print(" analogSecSum: ");
      //Serial.print(analogSecSum );
      
      //Serial.print(" Nr of samples: ");
      //Serial.print(j);
      
      Serial.print(" Average analogSecRead: ");
      Serial.print(analogSecSum / j);
      analogMinSum = analogMinSum + (analogSecSum / j);
      analogSecSum = 0;
      
      j = 0;
      if(secondsCounter == 60){  
        Serial.print(" Average analogMinRead: ");
        Serial.println(analogMinSum / 60);
        analogMinSum = 0;
            
        float averageCurrent = calculateAverageCurrent();
  
        secondsCounter = 0;  
        sigmaSum = 0;
  
        sendMeasuredData(averageCurrent);      
      }
    }
  }
}

void CurrentMeasure::analogReadEvery(unsigned long milisecTimePeriod){
  if((unsigned long)(millis() - analogReadStartTime) >= milisecTimePeriod) {
    sensorValue = analogRead(A0);
    inputStats.input(sensorValue);  
    analogReadStartTime = millis();
    j++;

    analogSecSum = analogSecSum + sensorValue;
  }
}

float CurrentMeasure::calculateAverageCurrent(){
  float sigmaAverage = sigmaSum/secondsCounter;
  Serial.print( "\n\t" );
  Serial.print( "sigma: " ); 
  //Serial.println();
  Serial.print(sigmaAverage);      

  float current_amps = (intercept + slope * sigmaAverage)*1000;
  Serial.print( "\n\t" );
  Serial.print( "mAmps: " ); 
  Serial.print( current_amps );

  return current_amps;
} 

void CurrentMeasure::sumSigma(){
  float sigma = inputStats.sigma();
    
  Serial.print("\n");
  Serial.print("sigma: "); 
  Serial.print(sigma);

  float test_amps = (intercept + slope * sigma)*1000;
  Serial.print("\tamps: "); 
  Serial.print(test_amps);

  sigmaSum = sigmaSum + sigma;
}

void CurrentMeasure::sendMeasuredData(float averageCurrent){
  if(averageCurrent > averageOfZeroAmps and mqtt.getState() == 0){

    String topic = "sockets/" + String(mqtt.clientId);
    char topicArray[topic.length() + 1];
    topic.toCharArray(topicArray, topic.length() + 1);
    
    mqtt.publish(topicArray, averageCurrent);  
  }
}

void CurrentMeasure::update(){
  measure();
}

