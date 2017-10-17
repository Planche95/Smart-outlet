
const char* mqttServer = "192.168.0.103";

void configureMqtt(){
  mqttClient.setServer(mqttServer, 1883);
}

boolean connectMqtt(){
  boolean isConnected = mqttClient.connect(Network::_ssidAccessPoint);
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
