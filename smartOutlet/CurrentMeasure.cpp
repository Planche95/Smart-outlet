#include "CurrentMeasure.h"

const float CurrentMeasure::windowLength = 1;
const float CurrentMeasure::intercept = -0.007;
const float CurrentMeasure::slope = 0.066;
const unsigned long CurrentMeasure::printPeriod = 1000;
const float CurrentMeasure::averageOfZeroAmps = 0.036;

CurrentMeasure::CurrentMeasure(Mqtt mqtt){
  this->mqtt = mqtt;
  inputStats.setWindowSecs(windowLength);
}

CurrentMeasure::CurrentMeasure(){
  
}

//TODO clean code
int j =0;
int initialDelay = 0;
float sumSigma2 = 0;

int samplesCounter = 0;
int values[251] = {0};

void CurrentMeasure::measure(){
  analogReadEvery(analogReadPeriod);
      
  if((unsigned long)(millis() - previousMillis) >= printPeriod) {
    previousMillis = millis();
    
    if(initialDelay < 5){
      initialDelay++;
      Serial.println("WAIT");
      j=0;
    } else{
      Serial.print("Nr of samples: ");
      Serial.print(j);

      if(j == 250 || j == 249 || j == 251){
        Serial.print("\t");

        calculateSigma();
        
        samplesCounter++;
        sumSigma();  
      } else {
        Serial.println();
      }
      
      secondsCounter++;    

      j = 0;
      if(secondsCounter == 60){  
        float averageCurrent = calculateAverageCurrent();

        samplesCounter = 0;
        secondsCounter = 0;  
        sigmaSum = 0;
        sumSigma2 = 0;
  
        sendMeasuredData(averageCurrent);      
      }
    }
  }
}

void CurrentMeasure::calculateSigma(){
  float tempSum = 0;
  float tempAverage = 0;
  
  for(int i = 0; i < j; i++){
    tempAverage = tempAverage + values[i];
  }

  tempAverage = tempAverage/j;

  for(int i = 0; i < j; i++){
    float tempDiff = (((float)values[i]) - tempAverage);
    tempSum = tempSum + (tempDiff * tempDiff);
  }

  float tempSigma = sqrt(tempSum/j);

  Serial.print(" sigma2: ");
  Serial.print(tempSigma);
  Serial.print(" ");

  sumSigma2 = sumSigma2 + tempSigma;

  //return sqrt(tempSum/j)
}

void CurrentMeasure::analogReadEvery(unsigned long milisecTimePeriod){
  if((unsigned long)(millis() - analogReadStartTime) >= milisecTimePeriod) {
    sensorValue = analogRead(A0);
    inputStats.input(sensorValue);  
    analogReadStartTime = millis();
    values[j] = sensorValue;
    j++;
  }
}

float CurrentMeasure::calculateAverageCurrent(){
  float sigmaAverage = sigmaSum/samplesCounter;
  Serial.print( "\n\t" );
  Serial.print( "sigma: " ); 
  Serial.print(sigmaAverage);
  
  float sigmaAverage2 = sumSigma2/samplesCounter;
  Serial.print( "\n\t" );
  Serial.print( "sigma2: " ); 
  Serial.print(sigmaAverage2);
        
  //mqtt.publish("socketsdata/14584001703309", sigmaAverage);
  //mqtt.publish("socketsdata/14584001703309", sigmaAverage2);
   
  float current_amps = (intercept + slope * sigmaAverage2);
  Serial.print( "\n\t" );
  Serial.print( "mAmps: " ); 
  Serial.print( current_amps );

  return current_amps;
} 

void CurrentMeasure::sumSigma(){
  float sigma = inputStats.sigma();
    
  //Serial.print("\n");
  Serial.print("sigma: "); 
  Serial.print(sigma);

  float test_amps = (intercept + slope * sigma)*1000;
  Serial.print("\tamps: "); 
  Serial.print(test_amps);
  Serial.print("\n");
  sigmaSum = sigmaSum + sigma;
}

void CurrentMeasure::sendMeasuredData(float averageCurrent){
  if(averageCurrent > averageOfZeroAmps and mqtt.getState() == 0){

    String topic = "socketsdata/" + String(mqtt.clientId);
    char topicArray[topic.length() + 1];
    topic.toCharArray(topicArray, topic.length() + 1);
    
    mqtt.publish(topicArray, averageCurrent * 230);  
  }
}

void CurrentMeasure::update(){
  measure();
}

