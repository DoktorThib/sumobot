/*
  SUMOBOT V1
    
  09/03/2019
  by Thibault Doktor & Anthony Delétang
 */
#include "ESC.h"
#define LED_PIN (13)                                      // Pin for the LED 
#define SPEED_MIN (1156)                                  // Set the Minimum Speed in microseconds
#define SPEED_MAX (1500)                                  // Set the Minimum Speed in microseconds

ESC myESC1 (2, SPEED_MIN, SPEED_MAX, 1000);                 // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
ESC myESC2 (3, SPEED_MIN, SPEED_MAX, 1000);                 // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)

int oESC;                                                 // Variable for the speed sent to the ESC
int strat = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);                               // LED Visual Output
  myESC1.arm();                                            // Send the Arm value so the ESC will be ready to take commands
  delay(500);
  myESC2.arm();                                            // Send the Arm value so the ESC will be ready to take commands
  digitalWrite(LED_PIN, HIGH);                            // LED High Once Armed
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
