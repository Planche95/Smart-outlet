#include "CurrentMeasure.h"

const float CurrentMeasure::windowLength = 20.0/60;
const float CurrentMeasure::intercept = -0.055;
const float CurrentMeasure::slope = 0.038;
const unsigned long CurrentMeasure::printPeriod = 1000;
const float CurrentMeasure::averageOfZeroAmps = 60.0;

CurrentMeasure::CurrentMeasure(Mqtt mqtt){
  this->mqtt = mqtt;
  inputStats.setWindowSecs(windowLength);
}

CurrentMeasure::CurrentMeasure(){
  
}

int i = 0;

void CurrentMeasure::measure(){
  if(i%1000 == 0){
    sensorValue = analogRead(A0);  // read the analog in value:
    inputStats.input(sensorValue);  // log to Stats function
    i = 1;
  }else{
    i++;
  }
      
  if((unsigned long)(millis() - previousMillis) >= printPeriod) {

    previousMillis = millis();
    sumSigma();
    secondsCounter++;    
    
    if(secondsCounter == 60){
      float averageCurrent = calculateAverageCurrent();

      secondsCounter = 0;  
      sigmaSum = 0;

      sendMeasuredData(averageCurrent);      
    }
  }
}

float CurrentMeasure::calculateAverageCurrent(){
  float sigmaAverage = sigmaSum/secondsCounter;
  Serial.print( "\n\t" );
  Serial.print( "sigma: " ); 
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

void CurrentMeasure::calibrate(){
  sensorValue = analogRead(A0);  
  inputStats.input(sensorValue);  
  samplesCounter++;
  
  if((unsigned long)(millis() - previousMillis) >= printPeriod) {
    previousMillis = millis();
    secondsCounter++;  
    Serial.print( "\n" );
    Serial.print("Probek: ");
    Serial.println(samplesCounter);
    samplesCounter = 0;
    Serial.print( inputStats.sigma() );
    if(secondsCounter == 60){
      Serial.println("");
      Serial.println("----------------- 60 sec");
      secondsCounter = 0;
    }
  }
}

void CurrentMeasure::update(){
  measure();
}

