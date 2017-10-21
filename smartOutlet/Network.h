#ifndef Network_h
#define Network_h

#include "AccessPoint.h"

class Network{
  
  private:
    String password;    
    String ssid;      
    char* passwordAccessPoint;    
    char* ssidAccessPoint;
    unsigned long waitForConnectStartTime = 0;
    unsigned long waitForConnectPeriod = 10000;
    
    //TODO change to interface 'Service'
    AccessPoint* accessPoint = NULL;

    void connect(String ssid, String password);
    void startService();
    void startAccessPoint();
    void startServer();
    void reconect();

  public: 
    Network(String ssid, String password, char* ssidAccessPoint, char* passwordAccessPoint);
    Network();
    void configure();
    bool isHost();
    bool isAccessPoint();
    void update();
};

#endif
