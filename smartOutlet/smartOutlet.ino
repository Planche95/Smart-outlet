#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include "Timer.h"


byte status;
//////////////////////EEPROM//////////////////////

const int ssidStartByte = 0;
const int passwordStartByte = 33;

const int ssidMaxLength = 32;
const int passwordMaxLength = 32;

void eepromSave(int from, int maxLength, String value){
  if(value.length() < maxLength){
    Serial.println("EEPROM: Write start " + value);
    
    EEPROM.begin(512);
    EEPROM.write(from, (byte)value.length());
    
    for(int i = from + 1, j = 0; j <= value.length(); i++, j++){
      EEPROM.write(i, value.charAt(j));
    }
    
    EEPROM.end();
    
    Serial.println("EEPROM: Write end");
  }
  else{
    Serial.println("EEPROM ERROR: Value is too long");
  }
}

String eepromRead(int from){
  EEPROM.begin(512);
  
  byte valueLength = EEPROM.read(from);

  if(valueLength != 255 and valueLength != 0){
    Serial.println("EEPROM: Read start");

    char readValue[valueLength + 1];
    
    Serial.print("Length: ");
    Serial.println(valueLength);
    
    for(int i = from + 1, j = 0; j < valueLength; i++, j++){
      readValue[j] = EEPROM.read(i);
    }

    readValue[valueLength] = '\0';
    EEPROM.end();
    
    String strReadValue = String(readValue);    
    Serial.print("value: ");
    Serial.println(readValue);
    Serial.println("EEPROM: Read end");
    
    
    return strReadValue;
  }
  else{
    Serial.println("EEPROM ERROR: No value is stored");
    
    EEPROM.end();
    return "";
  }
}

void readFirst66Bytes(){
  EEPROM.begin(512);
  for(int i = 0; i < 66; i++){
    char c = EEPROM.read(i);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(c);
    Serial.print(" : ");
    Serial.println((byte)c);
  }
  EEPROM.end();
}
//////////////////////////////////////////////////////////////////

//////////////////////ACCESS_POINT//////////////////////

const byte SIZE = 15;
char ssidAccessPoint[SIZE];
const char* passwordAccessPoint = "123456789";
const String ipAccessPoint = "192.168.4.1";

String command = "";

ESP8266WebServer server(80);

void configureSsid(){
  String ssidAP = String(ESP.getFlashChipId()) + String(ESP.getChipId());
  Serial.println("SSID: " + ssidAP);
  ssidAP.toCharArray(ssidAccessPoint,SIZE);
}

void startAccessPoint(){
  status = 0;
  WiFi.softAP(ssidAccessPoint,passwordAccessPoint);
  server.on("/",handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Access point!");  
}

void stopAccessPoint(){
  WiFi.softAPdisconnect(true);
}

void handleStartTimer(){
  startTimers();
  server.send(200,"text/html","Timer start!");
}

void handleRoot(){
  server.send(200,"text/html","Hello from esp8266! :" + String(ssidAccessPoint));
}

void handleStopTimer(){
  stopTimer();
  server.send(200,"text/html","Timer stop!");
}

void handleNotFound(){
  command=server.uri();
  server.send(200,"text/html",command);
}

const byte SIZE_SPLIT = 66;

void split(char* message, char** splitedMessage,  char* sign){
  char* split = strtok (message,sign);
  int i = 0;
  
  while (split != NULL)
  {
    splitedMessage[i++] = split;
    split = strtok (NULL, sign);
  }
}

void saveSsidAndPassword(){
  char path[SIZE_SPLIT];
  char *pathData[2];

  command.toCharArray(path,SIZE_SPLIT);
    
  split(path, pathData, "/");

  if(pathData[0] and pathData[1]){
    eepromSave(ssidStartByte, ssidMaxLength, pathData[0]);
    eepromSave(passwordStartByte, passwordMaxLength, pathData[1]);
    //Popracuj nad tymi statusami
    status = 1;
    Serial.println("Reset Arduino!");
    WiFi.softAPdisconnect(true);  
  }
  
}
//////////////////////////////////////////////////////////////

//////////////////////////MQTT////////////////////////////////////

WiFiClient client;
PubSubClient mqttClient(client);
const char* mqttServer = "192.168.0.103";

void configureMqtt(){
  mqttClient.setServer(mqttServer, 1883);
}

boolean connectMqtt(){
  boolean isConnected = mqttClient.connect(ssidAccessPoint);
  Serial.print("MQTT connected: ");
  Serial.println(isConnected);
  return isConnected;
}

void disconnectMqtt(){
  mqttClient.disconnect();
}

void isMqttconnected(){
  Serial.print("MQTT connected: ");
  Serial.println(mqttClient.connected());
}

void publishMqtt(char* topic, char* payload){
  //Will message - ostatnia wiadomosc przed rozlaczaniem! Moze sie przydac
  mqttClient.publish(topic, payload);
  Serial.println("Public message " + String(payload) + " on topic " + String(topic));
}
//////////////////////////////////////////////////////////////

///////////////////////////TIMER///////////////////////////////////

Timer timer;
int timerMilisecId;
int timerSecId;
int timerMinId;

int maxCurrent = -1;
float sumCurrent = 0.0;
int i = 0;

void timerLastWill(){
  Serial.println("This is Timer last will!");
}

int getCurrentTest(){
  return 312;
}

void readCurrent(void *context){
  int current = getCurrentTest();
  if(current > maxCurrent){
    maxCurrent = current;
  }
  
}

void addCurrent(void *context){
  i++;
  float amplitude_current = (float)(maxCurrent)/1024*5/185*1000000;
  float effective_value = amplitude_current/1.414;
  sumCurrent = sumCurrent + 10;
  maxCurrent = -1;
}

void sendAverageCurrent(void *context){
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
  
  char topic[8 + strlen(ssidAccessPoint)];
  strcpy(topic, "sockets/");
  strcat(topic, ssidAccessPoint);
  
  publishMqtt(topic, stringMessage); 
  Serial.print("i = ");
  Serial.println(i);
}

void startTimers(){
  connectMqtt();
  timerMilisecId = timer.every(100, readCurrent, (void*)0);
  timerSecId = timer.every(1000, addCurrent, (void*)0);
  timerMinId = timer.every(60000, sendAverageCurrent, (void*)0);
}

void stopTimer(){
  timer.stop(timerMilisecId);
  timer.stop(timerSecId);
  timer.stop(timerMinId);
  addCurrent((void*)0);
  sendAverageCurrent((void*)0);
  timerLastWill();
  disconnectMqtt();
}

//////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  Serial.println();
  configureSsid();
  configureMqtt();

  String ssidNetwork = eepromRead(ssidStartByte);
  String passwordNetwork = eepromRead(passwordStartByte);

  if(!ssidNetwork.equals("") and !passwordNetwork.equals("")){

    char ssidNetworkArray[ssidNetwork.length()]; 
    char passwordNetworkArray[passwordNetwork.length()];

    ssidNetwork.toCharArray(ssidNetworkArray, ssidNetwork.length() + 1);
    passwordNetwork.toCharArray(passwordNetworkArray, passwordNetwork.length() + 1);
    
    WiFi.begin(ssidNetworkArray, passwordNetworkArray);

    int i = 0;
    while (WiFi.status() != WL_CONNECTED and i++ < 20) {
      delay(500);
      Serial.print(".");
      Serial.println(WiFi.status());
    }
    
    
    if(WiFi.status() != WL_CONNECTED){
      startAccessPoint();    
    }
    else if(WiFi.status() == WL_CONNECTED){
      status = 1;
      Serial.print("Connected! IP: ");
      Serial.println(WiFi.localIP());
      server.on("/",handleRoot);
      server.on("/stop",handleStopTimer);
      server.on("/start",handleStartTimer);
      server.onNotFound(handleNotFound);
      server.begin();  
    } else {
      Serial.println("Other");
    }
  }
  else{
    Serial.println("Brak hasla i SSID");
    startAccessPoint();  
  }

  startTimers();
}

void loop() {
  server.handleClient();
  mqttClient.loop();
  timer.update();
  
  if(status == 0){
    if(!command.equals("")){
      Serial.println(command);
      saveSsidAndPassword();
      command = "";
    }
  } 
  
  //Jezeli odczyt z analog jest inny niz 2.5V start Timer
  //Jezeli odczyt == 2.5V wylacz timer, wykoncz wysylanie danych i rozlacz sie z MQTT  
}
