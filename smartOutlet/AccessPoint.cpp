#include "AccessPoint.h"
#include "Eeprom.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

char* AccessPoint::ssid;
char* AccessPoint::password;
ESP8266WebServer AccessPoint::server;

AccessPoint::AccessPoint(char* ssid, char* password){
  AccessPoint::ssid = ssid;
  AccessPoint::password = password; 
  AccessPoint::server = ESP8266WebServer(80);
}

void AccessPoint::handleRoot(){
  server.send(200,"text/html", "Hello from esp8266! :" + String(ssid));
}

void AccessPoint::handleNotFound(){
  server.send(200,"text/html", server.uri());
}

void AccessPoint::handleConfigure(){
  if(server.args() == 2){
    Eeprom::saveSsid(server.arg(0));
    Eeprom::savePassword(server.arg(1));
    Serial.println("Reset Arduino!");
    //TODO while(true) will reset?
    stop(); 
  }
  else{
    Serial.println("Niepoprawna liczba argumentow");
  }
}

void AccessPoint::configure(){
  WiFi.softAP(ssid, password);
  Serial.print("Ssid: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
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
