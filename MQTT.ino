#include <PubSubClient.h>

#include <ArduinoJson.h>


int mqttPort = 1883;
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[150];
char topicChar[150];
int value = 0;
int reconnectCount = 0;
StaticJsonDocument<220>  doc;


void callback(char* topic, byte* payload, unsigned int length) {
  int outputNo = -1;
  uint8_t value;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  int ledNo = 0;
  int subSize=0;
   while (subSize < sizeof(publisher) ) {
     subSize += sizeof(publisher[ledNo]);
     String subscription = publisher[ledNo]+"/g";
     if(subscription.equals(topic)) {
          break;
    }
    ledNo++;
  }
  String command = "";
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }
  Serial.print(command);
  Serial.print(" LED:");
  Serial.print(ledNo);
  if (command.equals("0")) {
    Serial.println(" OFF");  
    setLed(0,ledNo);
    LEDState[ledNo] = 0;
    
  } else {
    Serial.println(" ON");
    setLed(1,ledNo); 
    LEDState[ledNo] = 1;
  }
}

void reconnect() {
  // Loop until we're reconnected
  if (WiFi.status() != WL_CONNECTED) {
    initWIFI();
  }
  int mqttNo = 0;
  int mqttSize = 0;

  while (mqttSize < sizeof(mqtt_servers) ) {
    mqttSize += sizeof(mqtt_servers[mqttNo]);
    Serial.print(mqttNo);
    Serial.print(" MQTT Server :");
    Serial.println(mqtt_servers[mqttNo]);

    client.setServer( mqtt_servers[mqttNo], mqttPort);
    client.setCallback(callback);
    Serial.print("Connecting ");
    while (!client.connected() && reconnectCount < 3) {
      reconnectCount++;

      // Create a random client ID
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      Serial.print("Attempting MQTT connection... ");
      Serial.print(clientId);
      // Attempt to connect
      if (client.connect(clientId.c_str(), MQTTUser,  MQTTPassword)) {
        client.setCallback(callback);
        Serial.println("connected");
        reconnectCount = 0;
        // Once connected, publish an announcement...
        clientId.toCharArray(msg, clientId.length());
        //client.publish("ESPInit_SENSORBOARD", msg);
        // ... and resubscribe
        int subSize = 0;
        int subCount = 0;
        
        while (subSize < sizeof(publisher) ) {
          subSize += sizeof(publisher[subCount]);
          String subscription = publisher[subCount]+"/g";
          char copy[50];
          subscription.toCharArray(copy, 50);
          client.subscribe(copy);
          Serial.print("Subscribe: ");
          Serial.println(subscription);
          subCount++;
        }
        /*
         * while (subSize < sizeof(subscriptions) ) {
          subSize += sizeof(subscriptions[subCount]);
            char copy[50];
            subscriptions[subCount].toCharArray(copy, 50);
            client.subscribe(copy);
            Serial.print("Subscribe: ");
            Serial.println(subscriptions[subCount]);
              subCount++;
        
        }
       
         */
        break;
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
    if (client.connected()) {
      break;
    }
    reconnectCount = 0;
    mqttNo++;
  }
}


void initMQTT() {
  reconnect();
}

void handleMQTT() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

boolean LEDON( int no ) {
  boolean done = false;
  if (!client.connected()) {
    reconnect();
  }
  if (client.connected()) {
    Serial.print("Publish: ");
    Serial.print(publisher[no]);
    Serial.println(" ON: ");
    char copy[50];
    publisher[no].toCharArray(copy, 50);
    client.publish(copy, "ON");
    done = true;
  }
  return done;
}


boolean LEDOFF(int no) {
  boolean done = false;
  if (!client.connected()) {
    reconnect();
  }
  if (client.connected()) {
    Serial.print("Publish: ");
    Serial.print(publisher[no]);
    Serial.println(" OFF: ");
    char copy[50];
    publisher[no].toCharArray(copy, 50);
    client.publish(copy, "OFF");
    done = true;
  }
  return done;
}

boolean sendMQTT(String topic, String message  ) {
  boolean done = false;
  if (!client.connected()) {
    reconnect();
  }
  if (client.connected()) {
    Serial.print(topic);
    Serial.print(" Message: ");
    Serial.println(message);
    message.toCharArray(msg, message.length());
    topic.toCharArray(topicChar, topic.length());
    client.publish(topicChar, msg);
    done = true;
  }
  return done;
}
