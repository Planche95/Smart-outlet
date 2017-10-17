#ifndef Network_h
#define Network_h

#include "AccessPoint.h"

class Network{
  
  private:      
    static char* _passwordAccessPoint;    
    AccessPoint* accessPoint;

    void configureSsid();
    void connect(String ssid, String password);
    void startService();
    void startAccessPoint();
    void startServer();

  public: 
    //Jak sie tego pozbyc!!!
    static const int ssidEepromStartByte = 0;
    static const int passwordEepromStartByte = 33;
    static const int ssidEepromMaxLength = 32;
    static const int passwordEepromMaxLength = 32;

    static char _ssidAccessPoint[15];

    Network(char* passwordAccessPoint);
    void configure();
    void update();
};

#endif
