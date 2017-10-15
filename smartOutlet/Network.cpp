#include "Network.h"
#include "Host.h"
#include "AccessPoint.h"
#include "Eeprom.h"

void Network::configureSsid(){
  String ssidAP = String(ESP.getFlashChipId()) + String(ESP.getChipId());
  Serial.println("SSID: " + ssidAP);
  ssidAP.toCharArray(ssidAccessPoint, strlen(ssidAccessPoint));
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
  *accessPoint = AccessPoint(ssidAccessPoint, "123456789"); 
  (*accessPoint).configure();
  (*accessPoint).begin();   
}

void Network::startServer(){
  Host host(ssidAccessPoint);
  host.configure();
  host.begin();
}
 
void Network::configure(){
  String ssidNetwork = Eeprom::eepromRead(ssidEepromStartByte);
  String passwordNetwork = Eeprom::eepromRead(passwordEepromStartByte);

  if(!ssidNetwork.equals("") and !passwordNetwork.equals("")){
    connect(ssidNetwork, passwordNetwork);
    configureSsid();
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
