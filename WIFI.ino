//#include <WiFi.h>
#include <ESP8266WiFi.h>




//generate unique name from MAC addr
String macToStr(const uint8_t* mac){
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5){
      result += ':';
    }
  }
  return result;
}


String getFingerprint(const uint8_t* mac){
  String result;
  for (int i = 4; i < 6; ++i) {
    result += String(mac[i], 16);
  }
  return result;
}


void initWIFI() {

  
  //connect to WiFi
  int networkNo = 0;
  int ssidSize = 0;
  int retries = 12;
  int retry = 0;
  //attempt to connect to the wifi if connection is lost
  if (WiFi.status() != WL_CONNECTED) {

    // Try several WIFI Networks
    //for(networkNo = 0; networkNo < 10; networkNo++) {
    while (ssidSize < sizeof(ssid) ) {
      ssidSize += sizeof(ssid[networkNo]);
      Serial.print("Connecting ");
      Serial.print(networkNo);
      Serial.print(" to ");
      Serial.println(ssid[networkNo]);
      delay(100);
      WiFi.begin(ssid[networkNo], password[networkNo]);
      String clientName =  "LED_KEY";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += getFingerprint(mac);
      char host[clientName.length()];
      clientName.toCharArray(host, clientName.length());
      for (retry = 0; retry < retries; retry++) {
        if (WiFi.status() == WL_CONNECTED) {
          WiFi.hostname(host);
          break;
        } else {
          Serial.print(".");
          delay(1000);
        }
      }
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected as ");
        Serial.println(host);
        break;
      }
      networkNo++;
    }
  }
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
