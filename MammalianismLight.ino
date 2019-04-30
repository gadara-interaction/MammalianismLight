#include <AX12A.h>
#include <Stepper.h>
#include <DynamixelSerial.h>

#include "Light.h"
#define pwPin 13

//const int number_of_steps = 32;
const int number_of_steps = 8;
const int steps_output_rev = 32 * 64; // 2048  減速比 1/64
const int CW = 1;
const int CCW = -1;

Stepper stepperV(number_of_steps, 4, 6, 5, 7);//縦方向制御に使用するPIN(4-7)を指定
Stepper stepperH(number_of_steps, 8, 10, 9, 11);//横方向制御に使用するPIN(8-11)を指定

void setup() {
  pinMode(pwPin, INPUT);
  initLight();
  stepperV.setSpeed(1000);//１分間の回転数
  stepperH.setSpeed(1000);//１分間の回転数
  Dynamixel.begin(1000000,2);  // Inicialize the servo at 1Mbps and Pin Control 2
  delay(300);
  //Defaultの位置
  defaultPostion();
  delay(2000);
}

void loop() {
    unsigned long range_pw = pulseIn(pwPin, HIGH);
    int dist_inch = range_pw / 147; // 147us per inch
    unsigned int dist_cm = ((unsigned int)(dist_inch) * (unsigned int)(2.54 * 64)) >> 6;
    Serial.println(dist_cm); 
    defaultPostion();
    swingHorizontal();
    
    //もし距離センサに本が反応したら65cm以下にものがったらnod();
//    if(dist_cm <= 65){
//      nod();
//    }else{
//      defaultPostion();
//      swingHorizontal();
//      swingVertical(); 
//    }
}

void swingVertical(){
  int steps;
  steps = get_step_by_rev(CW, 0.07);
  stepperV.step(steps);
  steps = get_step_by_rev(CCW, 0.07);
  stepperV.step(steps);
}

void swingHorizontal(){
  int steps;
  steps = get_step_by_rev(CW, 0.3);
  stepperH.step(steps);
//  Dynamixel.moveSpeed(1,840,15);
  steps = get_step_by_rev(CCW, 0.3);
  stepperH.step(steps);
}

void nod(){
  Dynamixel.moveSpeed(1,780,30);
}

void defaultPostion(){
  Dynamixel.moveSpeed(1,880,10);

}


int get_step_by_rev(int dir, float rev){
  return dir * (steps_output_rev * rev);  
}
