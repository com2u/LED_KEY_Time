#include <Adafruit_NeoPixel.h>
#define LEDPin D2
#define NumberOfLEDs 60
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NumberOfLEDs, LEDPin, NEO_GRB + NEO_KHZ800);

void initLED() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void runLED(int r1, int g1, int b1, int r2, int g2, int b2) {
  strip.setPixelColor(0, strip.Color(r1, g1, b1));
  strip.setPixelColor(1, strip.Color(r2, g2, b2));
  strip.show(); 
}

void LEDStatus(int a, int b, int c) {
  strip.setPixelColor(0, strip.Color(a*40, 0, 0));
  strip.setPixelColor(1, strip.Color(b*40, 0, 0));
  strip.setPixelColor(2, strip.Color(c*40, 0, 0));
  strip.show(); 
}

void LightStripOn() {
  for (int i=0; i < NumberOfLEDs; i++) {
    strip.setPixelColor(i, strip.Color(30, 30, 30));
  }
  strip.show(); 
}

void LightStripOn(int i) {
  strip.setPixelColor(i, strip.Color(30, 30, 30));
  strip.show(); 
}

void LightStripOff() {
  for (int i=0; i < NumberOfLEDs; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show(); 
}
