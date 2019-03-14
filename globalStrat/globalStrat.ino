/*
  SUMOBOT V1
    
  14/03/2019
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
#define maxRangeLaserCaptor (750) //to define with test
#define triggerEnnemy (700)//Range to say to the code that we have an ennemy in front of him
#define distHandDetection (200)

VL6180X sensor; //declaration of the laser detector

ESC myESC1 (2, SPEED_MIN, SPEED_MAX, 1000);               // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
ESC myESC2 (3, SPEED_MIN, SPEED_MAX, 1000);               // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)

int strat = 0; //used later

//Change it to adapt your aggresivity
int mode = 0;//mode is 0, 1 or 2 <==> cool, normal, aggressive
boolean beginFight = false;

//pin here the captors are plugged (IR captors)
int IR_right = analogRead (A6);
int IR_left = analogRead (A7);

void setup() {
  setup_ESC();
  setup_laserCaptor();
  
}

//*************BEGIN OF THE LOOP*************//
void loop() {
  beginFight = triggeredFight();//block in a loop waiting for a hand in front of the captor
  if (triggeredFight() == true){//Begin of the "strategy"
    if (isBorder == true){
    if (seeEnnemy == true){
      thresholdAggressivity(mode);
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
}


//*************FUNCTIONS LINEAR MOUVEMENT*************//

void slowForward(){
  myESC1.speed(SPEED_MIN);
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

void search(){//to devellop, maybe, we will see the efficiency of a basic code
  speedForward(10); //slow deplacement
}

void attackFullPower(){//100%, we will see during test
  speedForward(100); //full speed to attack
}

void thresholdAggressivity(int mode){//mode is 0, 1 or 2 <==> cool, normal, aggressive
  //set, in case of bad use, a default value
  if (mode != 0 && mode != 1 && mode != 2)
    mode = 0;
  
  //make a linear acceleration, depends oof the distance of the ennemy
  switch(mode){
    case 0: linearAttack(25);
    break;
    
    case 1:linearAttack(50);
    break;
    
    case 2:linearAttack(100);
    break;
    
    default : linearAttack(25);//normally we will never access to this part of the code, cause of the if
    break;
  }
}

void linearAttack(int maxSpeedPercent){
  int dist = distEnnemy();
  int speed = (dist/maxRangeLaserCaptor)*maxSpeedPercent;//maybe use a global variable speed ? If dist = maxRangeLaserCaptor so it's full speed
  //maybe it will be to slow at the beginning, the ennemy will have the time to move, so a logarithmic acceleration will be better
  speedForward(speed);
  }

//*************FUNCTIONS ROTATION*************//

void rotateDegree (int degree, int speedPercent){ //enter a value between -180 and 180 included (clockwise rotation), 0 do nothing
  int delayRot = 0; //init to go forward in case of bad use
  //this two variables have to be set wirth some test to confirm it's linear (maybe try with 90° first and after others degrees)
  int coefDi = 10;//if it's linear
  int offset = 5;
  int coefSpeed = 1;
  
  delayRot = (coefDi * degree + offset)/(coefSpeed*speedPercent);//include variable speed to rotate at different speed
  
  //We have to know if it's a linear rotation, and etablish an equation to choose the good position
  if (degree == -180){
    rotateLeft180();
  }
  else if (degree == 180){
    rotateRight180();
  }
  else if (degree == 0){//do nothing
    
  }
  else if (degree > -180 && degree < 0){
    speedRotateLeft(speedPercent, delayRot);
  }
  else if (degree > 0 && degree < 180){
    speedRotateRight(speedPercent, delayRot);
  }
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

void rotateLeft180(){
  myESC2.speed(rotationSpeed);
  delay(delayRotate180);//delay to make 180° to left
  myESC2.stop();
}

void rotateRight180(){
  myESC1.speed(rotationSpeed);
  delay(delayRotate180);//delay to make 180° to left
  myESC2.stop();
}

void speedRotateRight(int speedPercent, int delayR){    //Speed in percent 0 MIN and 100 MAX
  int trueSpeed = SPEED_MIN + (speedPercent / 100)* (SPEED_MAX - SPEED_MIN);
  myESC1.speed(trueSpeed);
  delay(delayR);
}

void speedRotateLeft(int speedPercent,  int delayL){    //Speed in percent 0 MIN and 100 MAX
  int trueSpeed = SPEED_MIN + (speedPercent / 100)* (SPEED_MAX - SPEED_MIN);
  myESC2.speed(trueSpeed);
  delay(delayL);
}

//*************FUNCTIONS DETECTION*************//
boolean isBorder (){
  if (isBorderRight() || isBorderLeft()){
    return true;    
  }
  else{
    return false;
  }
}

boolean isBorderLeft(){
    if (analogRead(IR_left < thresholdDetection)){
    return true;    
  }
  else{
    return false;
  }
}

boolean isBorderRight(){
    if (analogRead(IR_right < thresholdDetection)){
    return true;    
  }
  else{
    return false;
  }
}

boolean seeEnnemy(){//put some step with a threshold ?
  if(sensor.readRangeContinuousMillimeters() < triggerEnnemy){
    return true;
  }
  else{
    return false;
  }
}

int distEnnemy (){ //function to keep a readable code
  return sensor.readRangeContinuousMillimeters();
}

//*************FUNCTIONS INTERACTION*************//
boolean triggeredFight(){
  int i = 0;
  while(1){//don't know if it's obligatory
    if (distEnnemy()< distHandDetection){
      for (i=0;i < 10;i++){
        delay(200);//10*200 ms = 2 seconds
        if (distEnnemy() > distHandDetection ){ //no more Hand in front of the captor
          i = 11 ; //equivalent to a break, but I prefer this, so we wait again for a hand
        }
        if (i==10){
          return true; //lauch the code to start a match
        }
      }
    }
  }
}

//*************FUNCTIONS SETUP*************//

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
