#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>      // this is needed even though we aren't using it
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <Servo.h>


Servo base, shoulder, elbow, wrist, twist, claw, myservo2; // initializes the servos
int base_theta, shoulder_theta, elbow_theta, wrist_theta, twist_theta, claw_theta;
int theta[] = {base_theta, shoulder_theta, elbow_theta, wrist_theta, twist_theta, claw_theta};


int start_pos[] = {90, 0, 90, 90, 0, 0}; // set for all 6 servos uses; Base, Shoulder, Elbow, Wrist, Twist, Claw

int val = 0;
int state_duration = 30*1000;

unsigned long previousMillis = 0;
int i = 0;

int state = -1;
int time_val;

void setup() {

  Serial.begin(9600);
  Serial.println(F("CHIROID Program Started"));
  
  base.attach(2);
  shoulder.attach(3);
  elbow.attach(4);
  wrist.attach(5);
  Serial.println("Servos have been assigned pins");

  //set the servos to the initial positions
  base.write(start_pos[0]);
  shoulder.write(start_pos[1]);
  elbow.write(start_pos[2]);
  wrist.write(start_pos[3]);

  for (int i = 0; i < 4; i++) {
    theta[i] = start_pos[i];
  }
  Serial.println("Servos are at their start positions");

}

void loop() {
  unsigned long time_MAIN = millis();
  time_val = (int(floor(time_MAIN/1000))%30);
  int timetochange = 2;
  
  if(time_val==0){
    Serial.println("---------------------------------------------------------------------");
    //Serial.print("Time shown: "); Serial.println(time_MAIN);
    state+=1;
    if(state<5){
      state_Servo(state, timetochange);
      delay(1000);
    }else{
      state = 0;
      state_Servo(state, timetochange);
      delay(1000);
    }
  }
  //time_MAIN -= timetochange;
}

void state_Servo(int state, int time_dif) {
   //DEFAULT: 2 seconds for the arm to change values
  if (state == 0 or state == 4){ 
    mvServo(theta[1], 0, time_dif);
    theta[1] = 0;
    Serial.println("State: Sit");
  }else if(state == 1 or state == 3){
    mvServo(theta[1], 45, time_dif);
    theta[1] = 45;
    Serial.println("State: Perch");
  }else if(state==2){
    mvServo(theta[1], 90, time_dif);
    theta[1] = 90;
    Serial.println("State: Stand");
  }
  Serial.print("CHIROID reports: ");
  Serial.println(shoulder.read());
  
}

void mvServo(int old_val, int new_val, int time_dif){
  int sign = (new_val - old_val)/abs(new_val - old_val);
  if(new_val != old_val){
    for(old_val; old_val != new_val; old_val+= sign*1){
      shoulder.write(old_val);
       Serial.print(shoulder.read()); Serial.print('\t');Serial.println(millis());
      delay((time_dif*1000)/45); //robot arm changes a degree at the time interval in the delay fn
    }
  shoulder.write(new_val);
  }
}


