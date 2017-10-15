#include "AccessPoint.h"
#include "Eeprom.h"
#include "Network.h"

void AccessPoint::handleRoot(){
  server.send(200,"text/html", "Hello from esp8266! :" + String(ssid));
}

void AccessPoint::handleNotFound(){
  server.send(200,"text/html", server.uri());
}

void AccessPoint::handleConfigure(){
  if(server.args() == 2){
    saveSsidAndPassword(server.arg(0), server.arg(1));
  }
  else{
    Serial.println("Niepoprawna liczba argumentow");
  }
}

void AccessPoint::saveSsidAndPassword(String ssid, String password){
  char ssidArray[ssid.length()]; 
  char passwordArray[password.length()];

  ssid.toCharArray(ssidArray, ssid.length() + 1);
  password.toCharArray(passwordArray, password.length() + 1);

  //Eeprom::eepromSave(Network::ssidEepromStartByte, Network::ssidEepromMaxLength, ssidArray);
  //Eeprom::eepromSave(Network::passwordEepromStartByte, Network::passwordEepromMaxLength, passwordArray);
  Serial.println("Reset Arduino!");
  stop();  
}

AccessPoint::AccessPoint(char* ssid, char* password){
  this->ssid = ssid;
  this->password = password; 
  server = ESP8266WebServer(80);
}

void AccessPoint::configure(){
  WiFi.softAP(ssid,password);
  server.on("/",handleRoot);
  server.on("/configure", handleConfigure);
  server.onNotFound(handleNotFound);
}
  
void AccessPoint::begin(){
  server.begin();
  Serial.println("Access point!");  
}

void AccessPoint::stop(){
  WiFi.softAPdisconnect(true);
}

void AccessPoint::handleClient(){
  server.handleClient();
}
