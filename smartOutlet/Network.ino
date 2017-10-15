class AccessPoint{
  private:
  static ESP8266WebServer server;  
  static char* ssid;
  char* password;

  static void handleRoot(){
    server.send(200,"text/html","Hello from esp8266! :" + String(ssid));
  }

  static void handleNotFound(){
    server.send(200,"text/html", server.uri());
  }

  public:
  AccessPoint(char* ssid, char* password){
    this->ssid = ssid;
    this->password = password; 
    server = ESP8266WebServer(80);
  }
  
  void begin(){
    WiFi.softAP(ssid,password);
    server.on("/",handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("Access point!");  
  }

  void stop(){
    WiFi.softAPdisconnect(true);
  }
};

class Host{

  private:
  static ESP8266WebServer server;

  static void handleStartTimer(){
    startTimers();
    server.send(200,"text/html","Timer start!");
  }

  static void handleRoot(){
    server.send(200,"text/html","Hello from esp8266! :" + String(ssidAccessPoint));
  }

  static void handleStopTimer(){
    stopTimer();
    server.send(200,"text/html","Timer stop!");
  }

  static void handleNotFound(){
    command=server.uri();
    server.send(200,"text/html",command);
  }

  public:
  Host(){
    server = ESP8266WebServer(80);
  }
  
  void configure(){
    Serial.print("Connected! IP: ");
    Serial.println(WiFi.localIP());
    server.on("/",handleRoot);
    server.on("/stop",handleStopTimer);
    server.on("/start",handleStartTimer);
    server.onNotFound(handleNotFound);
  }

  void begin(){
    server.begin();
  }
};

class Network{
  
  private:
  byte status;

  const int ssidStartByte = 0;
  const int passwordStartByte = 33;

  const int ssidMaxLength = 32;
  const int passwordMaxLength = 32;

  char ssidAccessPoint[15];
  const char* passwordAccessPoint = "123456789";

  void configureSsid(){
    String ssidAP = String(ESP.getFlashChipId()) + String(ESP.getChipId());
    Serial.println("SSID: " + ssidAP);
    ssidAP.toCharArray(ssidAccessPoint, strlen(ssidAccessPoint));
  }

  void connect(String ssid, String password){
    
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

    if(WiFi.status() != WL_CONNECTED){
      AccessPoint accessPoint("test", "test");    
    }
    else if(WiFi.status() == WL_CONNECTED){
      status = 1;
      Host host;
      host.configure();
      host.begin();
    } 
    else {
      Serial.println("Other");
    }
}

  public: 
  void configure(){
    String ssidNetwork = eepromRead(ssidStartByte);
    String passwordNetwork = eepromRead(passwordStartByte);

    if(!ssidNetwork.equals("") and !passwordNetwork.equals("")){
      connect(ssidNetwork, passwordNetwork);
    }
    else{
      Serial.println("Brak hasla i SSID");
      //startAccessPoint();  
    }
  }
};

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
