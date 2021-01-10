#include <ESP8266HTTPClient.h>

void postHTTP(String postURL, String postMessage){
    
  Serial.print(postURL);
  Serial.println(postMessage);
  //Declare an object of class HTTPClient
  HTTPClient http;

  if (WiFi.status() == WL_CONNECTED) {
    //Specify request destination
    http.begin(postURL);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("auth-key", "My_authentication_key");
    //Send the request
    int httpCode = http.POST(postMessage);
    http.end();
    
    /*
    //Check the returning code
    if (httpCode > 0) {
      //Get the request response payload
      String payload = http.getString();
      //Print the response payload
      Serial.println(payload);
    }
    Serial.println(httpCode);
    */
  }
}
