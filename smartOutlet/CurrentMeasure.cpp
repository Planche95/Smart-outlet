#include "CurrentMeasure.h"

const float CurrentMeasure::windowLength = 20.0/60;
const float CurrentMeasure::intercept = -0.1310;
const float CurrentMeasure::slope = 0.04099;
const unsigned long CurrentMeasure::printPeriod = 1000;

CurrentMeasure::CurrentMeasure(Mqtt mqtt){
  this->mqtt = mqtt;
  inputStats.setWindowSecs(windowLength);
}

CurrentMeasure::CurrentMeasure(){
  
}

void CurrentMeasure::measure(){
  sensorValue = analogRead(A0);  // read the analog in value:
  inputStats.input(sensorValue);  // log to Stats function
      
  if((unsigned long)(millis() - previousMillis) >= printPeriod) {
    previousMillis = millis();

    float sigma = inputStats.sigma();
    
    Serial.print("\n");
    Serial.print("sigma: "); 
    Serial.print(sigma);
    //current_amps = (intercept + slope * sig)*1000;
    //Serial.print( "\tamps: " ); Serial.print( current_amps );    
    secondsCounter++;    
    sigmaSum = sigmaSum + sigma;
    
    if(secondsCounter == 60){
      float sigmaAverage = sigmaSum/secondsCounter;
      Serial.print( "\n\t" );
      Serial.print( "sigma: " ); 
      Serial.print(sigmaAverage);      

      float current_amps = (intercept + slope * sigmaAverage)*1000;
      Serial.print( "\n\t" );
      Serial.print( "mAmps: " ); 
      Serial.print( current_amps );

      secondsCounter = 0;  
      sigmaSum = 0;
      
      //If connected to server and bigger than the 0amp sigma send!
      //mqtt.publish(topic, stringMessage);
    }
  }
}

 

void CurrentMeasure::calibrate(){
  sensorValue = analogRead(A0);  // read the analog in value:
  inputStats.input(sensorValue);  // log to Stats function
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

void averageSigma(){
   
}

void CurrentMeasure::update(){
  
}

