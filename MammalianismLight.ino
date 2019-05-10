/*
   配線:

   Dynamixel - モーター
   - TX,GND,5V

   Steppingモータ-
   左右 - D4,D5,D6,D7
    -上下 D8,D9,D10,D11

   D13: 超音波センサ

*/

#include <AX12A.h>
#include <Stepper.h>
#include <DynamixelSerial.h>

#include "Light.h"
#define pwPin 13

const int number_of_steps = 8;
const int number_of_steps_lr = 4;
const int steps_output_rev = 32 * 64; // 2048  減速比 1/64
const int CW = 1;
const int CCW = -1;

bool book_mode = false; //前回nodしたかどうかの判定用（まっすぐお辞儀させるにはどうすればいいか試した）

Stepper stepperV(number_of_steps_lr, 4, 6, 5, 7);//縦方向制御に使用するPIN(4-7)を指定

Stepper stepperH(number_of_steps, 8, 10, 9, 11);//横方向制御に使用するPIN(8-11)を指定

//--- パラメーター調整するところ
float left_right_para = 0.16; //0.03~0.08ぐらいの間で調整すると良い
float up_down_para = 0.08; //0.1~0.5ぐらいの間で調整すると良い。基本いじらなくて大丈夫だと思う

int default_pos = 830; //サーボーのデフォルト位置 800~850ぐらいの間で調整すると良い
int nod_pos = 760; //本をかざしたときのサーボーの位置

int distance = 45; //超音波センサが反応する距離cm

//----


void setup() {
  pinMode(pwPin, INPUT);
  initLight();
  stepperV.setSpeed(1000);//１分間の回転数
  stepperH.setSpeed(1000);//１分間の回転数
  Dynamixel.begin(1000000, 2); // Inicialize the servo at 1Mbps and Pin Control 2
  delay(300);
  calibration();

  delay(2000);
}

void loop() {
  unsigned long range_pw = pulseIn(pwPin, HIGH);
  int dist_inch = range_pw / 147; // 147us per inch
  unsigned int dist_cm = ((unsigned int)(dist_inch) * (unsigned int)(2.54 * 64)) >> 6;
  Serial.println(dist_cm);

  if (dist_cm <= distance) {
    //本があるとき
    nod();
    book_mode = true;
    
  } else {
    //本がないとき
     calibration();
 //   defaultPostion();
    move_mammal();
    book_mode = false;
  }
}

void move_mammal() {
  //  swingVertical();
  int r = random(10);
  if (r >= 9) {
    //チューチュートレイン
    //      naturalBehaviour();
    swingVertical();
  } else if (r >= 4) {
    //横の動き
    swingVertical();
  } else {
    //縦の動き
    //      blinkLight();
    int steps;
    steps = get_step_by_rev(CW, left_right_para/2 );
    stepperV.step(steps);
    swingHorizontal();
  }
}

void calibration() {
  //サーボーモーターの位置キャリブ
  defaultPostion();
  //ステッピングモーターのキャリブ
  int steps;
  steps = get_step_by_rev(CCW, left_right_para / 2 );
  stepperV.step(steps);
}

void naturalBehaviour() {
  int vsteps, hsteps;
  //右
  vsteps = get_step_by_rev(CW, left_right_para);
  stepperV.step(vsteps);
  //上
  hsteps = get_step_by_rev(CW, up_down_para / 2);
  stepperH.step(hsteps);
  //左
  vsteps = get_step_by_rev(CCW, left_right_para);
  stepperV.step(vsteps);
  //下
  hsteps = get_step_by_rev(CCW, up_down_para / 2);
  stepperH.step(hsteps);
}


void swingVertical() {
  int steps;
  steps = get_step_by_rev(CW, left_right_para);
  stepperV.step(steps);
  steps = get_step_by_rev(CCW, left_right_para);
  stepperV.step(steps);
}

void swingHorizontal() {
  int steps;
  steps = get_step_by_rev(CW, up_down_para);
  stepperH.step(steps);
  steps = get_step_by_rev(CCW, up_down_para);
  stepperH.step(steps);
}

//本をかざしたとき
void nod() {
  int steps;
  //(ID,位置,スピード) 基本IDは固定
  if (book_mode == false) {
    steps = get_step_by_rev(CW, left_right_para / 2 ); //ステッピングモータを中央に戻す
    stepperV.step(steps);
    Dynamixel.moveSpeed(1, nod_pos, 25);
  } else {
    Dynamixel.moveSpeed(1, nod_pos, 25);
  }
  //  steps = get_step_by_rev(CW, left_right_para/2 );
  //  stepperV.step(steps);
}

void defaultPostion() {
  //(ID,位置,スピード) 基本IDは固定
  Dynamixel.moveSpeed(1, default_pos, 10);
}


int get_step_by_rev(int dir, float rev) {
  return dir * (steps_output_rev * rev);
}
