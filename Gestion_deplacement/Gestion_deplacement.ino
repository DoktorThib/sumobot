/*
  SUMOBOT V1
    
  09/03/2019
  by Thibault Doktor & Anthony Delétang
 */
#include "ESC.h"

#define SPEED_MIN (1156)                                  // Set the Minimum Speed in microseconds
#define SPEED_MAX (1500)                                  // Set the Minimum Speed in microseconds

ESC myESC1 (2, SPEED_MIN, SPEED_MAX, 1000);               // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
ESC myESC2 (3, SPEED_MIN, SPEED_MAX, 1000);               // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)

int oESC;                                                 // Variable for the speed sent to the ESC
int strat = 0;

void setup() {
  myESC1.arm();                                            // Send the Arm value so the ESC will be ready to take commands
  myESC2.arm();                                            // Send the Arm value so the ESC will be ready to take commands
  delay(2000);
}

void loop() {
  slowForward();
}

void slowForward(){
  myESC1.speed(SPEED_MIN);
  myESC2.speed(SPEED_MIN);
  delay(20);
}

void rotateRight(){
  myESC1.speed(SPEED_MIN);
  myESC2.stop();
  delay(20);
}

void rotateLeft(){
  myESC1.stop();
  myESC2.speed(SPEED_MIN);
  delay(20);
}

void stopRobot(){
  myESC1.stop();
  myESC2.stop();
  delay(20);
}

void speedForward(int speedPercent){    //Speed in percent 0 MIN and 100 MAX
  int trueSpeed = SPEED_MIN + (speedPercent / 100)* (SPEED_MAX - SPEED_MIN);
  myESC1.speed(trueSpeed);
  myESC2.speed(trueSpeed);
  delay(20);
}

void speedRotateRight(int speedPercent){    //Speed in percent 0 MIN and 100 MAX
  int trueSpeed = SPEED_MIN + (speedPercent / 100)* (SPEED_MAX - SPEED_MIN);
  myESC1.speed(trueSpeed);
  delay(20);
}

void speedRotateLeft(int speedPercent){    //Speed in percent 0 MIN and 100 MAX
  int trueSpeed = SPEED_MIN + (speedPercent / 100)* (SPEED_MAX - SPEED_MIN);
  myESC2.speed(trueSpeed);
  delay(20);
}
