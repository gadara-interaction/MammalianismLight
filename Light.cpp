#include "Light.h"
#include <Adafruit_NeoPixel.h>

//LED 3つ, D2に接続
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(3, 2, NEO_GRB + NEO_KHZ800);

void initLight();


void initLight(){
  pixel.begin();
  pixel.setPixelColor(0, pixel.Color(255,168,97)); //一番最初のLEDは０番です
  pixel.setPixelColor(1, pixel.Color(255,168,97)); //一番最初のLEDは０番です
  pixel.setPixelColor(2, pixel.Color(255,168,97)); //一番最初のLEDは０番です
  pixel.setBrightness(255);
  pixel.show();
}

