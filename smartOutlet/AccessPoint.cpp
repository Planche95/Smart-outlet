#include "AccessPoint.h"
#include "Eeprom.h"
#include "Network.h"

char* AccessPoint::_ssid;
char* AccessPoint::_password;
ESP8266WebServer AccessPoint::_server;

void AccessPoint::handleRoot(){
  _server.send(200,"text/html", "Hello from esp8266! :" + String(_ssid));
}

void AccessPoint::handleNotFound(){
  _server.send(200,"text/html", _server.uri());
}

void AccessPoint::handleConfigure(){
  if(_server.args() == 2){
    saveSsidAndPassword(_server.arg(0), _server.arg(1));
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

  Eeprom::eepromSave(Network::ssidEepromStartByte, Network::ssidEepromMaxLength, ssidArray);
  Eeprom::eepromSave(Network::passwordEepromStartByte, Network::passwordEepromMaxLength, passwordArray);
  Serial.println("Reset Arduino!");
  stop();  
}

AccessPoint::AccessPoint(char* ssid, char* password){
  _ssid = ssid;
  _password = password; 
  _server = ESP8266WebServer(80);
}

void AccessPoint::configure(){
  WiFi.softAP(_ssid, _password);
  _server.on("/",handleRoot);
  _server.on("/configure", handleConfigure);
  _server.onNotFound(handleNotFound);
}
  
void AccessPoint::begin(){
  _server.begin();
  Serial.println("Access point!");  
}

void AccessPoint::stop(){
  WiFi.softAPdisconnect(true);
}

void AccessPoint::handleClient(){
  _server.handleClient();
}
