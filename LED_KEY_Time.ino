#include <ESP8266WiFi.h>
#include <Credentials.h>
#include "wiring_shift_mod.h"


String publisher[] = {"wled/e19b87", "wled/tisch", "wled/bad", "wled/1c2d0d", "wled/tv", "wled/buero", "b", "c", "d"};
String postURL[] = {"http://192.168.0.101/json/si","http://192.168.0.104/json/si","http://192.168.0.78/json/si","http://192.168.0.112/json/si","http://192.168.0.109/json/si","http://192.168.0.114/json/si","http://192.168.0.78/json/si"};
String effectList[] = {"{seg: {fx: 0}, v: true}", 
                       "{seg: {col: [[255, 0, 0, \"0\"], [], []]}, transition: 7, v: true}",
                       "{seg: {col: [[255, 255, 0, \"0\"], [], []]}, transition: 7, v: true}",
                       "{seg: {col: [[0, 255, 0, \"0\"], [], []]}, transition: 7, v: true}",
                       "{seg: {col: [[0, 255, 255, \"0\"], [], []]}, transition: 7, v: true}", 
                       "{seg: {col: [[0, 0, 255, \"0\"], [], []]}, transition: 7, v: true}", 
                       "{seg: {col: [[255, 0, 255, \"0\"], [], []]}, transition: 7, v: true}", 
                       "{seg: {col: [[255, 255, 255, \"0\"], [], []]}, transition: 7, v: true}", 
                       "{seg: {fx: 71}, v: true}" };
int currecntEffect = 0;

byte actualHour = 24;
byte actualMinute = 60;
byte actualsecond = 60;
int actualyear = 2020;
byte actualMonth = 11;
byte actualday =16;
byte actualdayofWeek = 1;

int LEDState[] = {0,0,0,0,0,0,0,0};
int debounce[] = {0,0,0,0,0,0,0,0};
uint8_t buttons;


void setup()
{
  initLEDKEY();
  Serial.begin(115200);
  delay(50);
  initWIFI();
  delay(50);
  initTime();
  initLED();
  initMQTT();
}

void loop()
{
    segment7clock();
    handleMQTT();
    for (int x = 0 ; x < 40; x++) {
      buttons = readButtons();
      //Serial.print(buttons);  
      processButtons(buttons);
      delay(10);
    }
    LightStripOff();
}

void processButtons(uint8_t buttons){
    if (buttons!=0 ) {
      Serial.print("Buttons:"); 
      Serial.print(buttons); 
      for (int i = 0; i< 8; i++){
          if (buttons==(pow(2,i))) {
            if (debounce[i] == 0) {
              Serial.print(" LEDState");  
              Serial.print(i);  
              Serial.print(" ");  
              Serial.print(LEDState[i]);  
              if(LEDState[i]==1) {
                LEDOFF(i);
              } else {
                LEDON(i);
              }
            }
            debounce[i]++;
            Serial.print(" Debounce:");  
            Serial.println(debounce[i]);  
            if (debounce[i] % 10 == 0) {
              postHTTP(postURL[i],effectList[currecntEffect]);
              currecntEffect++;
              if (currecntEffect>9) {
                currecntEffect = 0;
              }
              delay(200);
            }
            delay(200);
          }
        }

    }
    else {  // no button pressed
      for (int i = 0; i< 8; i++){
        if (debounce[i] > 0) {
          debounce[i] = 0;
          delay(200);
        }
      }
    }
}
