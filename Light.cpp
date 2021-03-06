#include "Light.h"
#include <Adafruit_NeoPixel.h>

//LED 3つ, D2に接続
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(5, 2, NEO_GRB + NEO_KHZ800);

//増やす場合
//Adafruit_NeoPixel pixel = Adafruit_NeoPixel(4, 2, NEO_GRB + NEO_KHZ800);

void initLight();
void blinkLight();

int blink_count = 0;

void initLight(){
  pixel.begin();
  pixel.setPixelColor(0, pixel.Color(255,168,97)); //一番最初のLEDは０番です
  pixel.setPixelColor(1, pixel.Color(255,168,97)); //一番最初のLEDは０番です
  pixel.setPixelColor(2, pixel.Color(255,168,97)); //一番最初のLEDは０番です
  pixel.setPixelColor(3, pixel.Color(255,168,97)); //一番最初のLEDは０番です
  pixel.setPixelColor(4, pixel.Color(255,168,97));
  pixel.setBrightness(255);
  pixel.show();
}

void blinkLight(){

}
