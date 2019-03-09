/*
  SUMOBOT V1
    
  09/03/2019
  by Thibault Doktor & Anthony Delétang
 */
#include "ESC.h"
#include <Wire.h>
#include <VL6180X.h>

#define SPEED_MIN (1156)                                  // Set the Minimum Speed in microseconds
#define SPEED_MAX (1500)                                  // Set the Minimum Speed in microseconds
#define delayRotate180 (150)   //set the time to rotate
#define rotationSpeed (1156)    //define the time what you need to rotate, depends of delayRotate
#define thresholdDetection (120) //define threshold of IR captor

VL6180X sensor; //declaration of the laser detector

ESC myESC1 (2, SPEED_MIN, SPEED_MAX, 1000);               // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
ESC myESC2 (3, SPEED_MIN, SPEED_MAX, 1000);               // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)

int strat = 0; //used later

//pin here the captors are plugged (IR captors)
int IR_right = analogRead (A6);
int IR_left = analogRead (A7);

void setup() {
  setup_ESC();
  setup_laserCaptor();
  
}
//*************BEGIN OF THE LOOP*************//
void loop() {
  if (isBorder == true){
    if (seeEnnemy == true){
      attack();
    }
    else{
      search();
    }
  }
  else{
    stopRobot();
    rotateRight180();
  }
}
//*************END OF THE LOOP*************//

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

void rotateLeft180(){    //Speed in percent 0 MIN and 100 MAX
  myESC2.speed(rotationSpeed);
  delay(delayRotate180);//delay to make 180° to left
  myESC2.stop();
}

void rotateRight180(){    //Speed in percent 0 MIN and 100 MAX
  myESC1.speed(rotationSpeed);
  delay(delayRotate180);//delay to make 180° to left
  myESC2.stop();
}

void setup_ESC(){
  myESC1.arm();                                            // Send the Arm value so the ESC will be ready to take commands
  myESC2.arm();                                            // Send the Arm value so the ESC will be ready to take commands
  delay(2000);  
}

void setup_lineCaptors(int pinCaptor1,int pinCaptor2){
  
}

void setup_laserCaptor(){
  Wire.begin(); //for I2C
  
  sensor.init();
  sensor.configureDefault();

  // Reduce range max convergence time and ALS integration
  // time to 30 ms and 50 ms, respectively, to allow 10 Hz
  // operation (as suggested by Table 6 ("Interleaved mode
  // limits (10 Hz operation)") in the datasheet).
  sensor.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  sensor.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);

  sensor.setTimeout(500);

   // stop continuous mode if already active
  sensor.stopContinuous();
  // in case stopContinuous() triggered a single-shot
  // measurement, wait for it to complete
  delay(300);
  // start interleaved continuous mode with period of 100 ms
  sensor.startInterleavedContinuous(100);
  //increase range
  sensor.setScaling(3);  
}

boolean isBorder (){
  if (analogRead(IR_right < thresholdDetection || IR_left < thresholdDetection)){
    return true;    
  }
  else{
    return false;
  }
}

boolean seeEnnemy(){//put some step with a threshold ?
  if(sensor.readRangeContinuousMillimeters()<700){
    return true;
  }
  else{
    return false;
  }
}

void search(){//to devellop, maybe, we will see the efficiency of a basic code
  speedForward(10); //slow deplacement
}

void attack(){//Maybe not 100% we will see during test
  speedForward(100); //full speed to attack
}
