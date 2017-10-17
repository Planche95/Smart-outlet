#include "Network.h"
#include "Host.h"
#include "AccessPoint.h"
#include "Eeprom.h"

char Network::_ssidAccessPoint[15];  
char* Network::_passwordAccessPoint;

Network::Network(char* passwordAccessPoint){
  _passwordAccessPoint = passwordAccessPoint;
  configureSsid();
}

void Network::configureSsid(){
  String ssidAP = String(ESP.getFlashChipId()) + String(ESP.getChipId());
  Serial.println("SSID: " + ssidAP);
  ssidAP.toCharArray(_ssidAccessPoint, strlen(_ssidAccessPoint));
}

void Network::connect(String ssid, String password){
    
  char ssidArray[ssid.length()]; 
  char passwordArray[password.length()];

  ssid.toCharArray(ssidArray, ssid.length() + 1);
  password.toCharArray(passwordArray, password.length() + 1);
    
  WiFi.begin(ssidArray, passwordArray);

  int waitForConnect = 0;
  while (WiFi.status() != WL_CONNECTED and waitForConnect++ < 20) {
    delay(500);
    Serial.print(".");
    Serial.println(WiFi.status());
  }
}

void Network::startService(){
  if(WiFi.status() == WL_CONNECTED){
    startServer();
  }
  else{
    startAccessPoint();
  }
}

void Network::startAccessPoint(){
  *accessPoint = AccessPoint(_ssidAccessPoint, _passwordAccessPoint); 
  (*accessPoint).configure();
  (*accessPoint).begin();   
}

void Network::startServer(){
  Host host(_ssidAccessPoint);
  host.configure();
  host.begin();
}
 
void Network::configure(){
  String ssidNetwork = Eeprom::eepromRead(ssidEepromStartByte);
  String passwordNetwork = Eeprom::eepromRead(passwordEepromStartByte);

  if(!ssidNetwork.equals("") and !passwordNetwork.equals("")){
    connect(ssidNetwork, passwordNetwork);
    startService();
  }
  else{
    Serial.println("Brak hasla i SSID");
    startAccessPoint();  
  }
}

void Network::update(){
  if(accessPoint != NULL){
    (*accessPoint).handleClient();  
  }
}
