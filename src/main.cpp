#include <Arduino.h>
#include <NewPing.h>
#define M_RIGHT_SPEED 10
#define M_RIGHT_1 8
#define M_RIGHT_2 9
#define M_LEFT_SPEED 11
#define M_LEFT_1 1
#define M_LEFT_2 0
#define U_RIGHT_ECHO A3
#define U_RIGHT_TRIG A2
#define U_LEFT_ECHO A5
#define U_LEFT_TRIG A4
#define U_FRONT_ECHO A1
#define U_FRONT_TRIG A0
#define STD_SPEED 150


#define LEFT_ADD_SPEED 10
#define RIGHT_ADD_SPEED -10
#define MAX_DISTANCE 200
#define P 18
#define I 0
#define D 5
NewPing uRight(U_RIGHT_TRIG, U_RIGHT_ECHO, MAX_DISTANCE);
NewPing uLeft(U_LEFT_TRIG, U_LEFT_ECHO, MAX_DISTANCE);
NewPing uFront(U_FRONT_TRIG, U_FRONT_ECHO, MAX_DISTANCE);
void motor(int,int);
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void setup() {
  // put your setup code here, to run once:
  pinMode(M_RIGHT_1,OUTPUT);
  pinMode(M_RIGHT_2,OUTPUT);
  pinMode(M_LEFT_1,OUTPUT);
  pinMode(M_LEFT_2,OUTPUT);
  pinMode(M_RIGHT_SPEED,OUTPUT);
  pinMode(M_LEFT_SPEED,OUTPUT);
  //Serial.begin(9600);
}
int distances[3]={};
void getDistances(int*);
void PID();
void loop() {
  // put your main code here, to run repeatedly:
  /*
  getDistances(&distances[0]);
  Serial.print(distances[0]);
  Serial.print(",");
  Serial.print(distances[1]);
  Serial.print(",");
  Serial.println(distances[2]);
  */
  moveForward();
  PID();
  


}

void motor(int index, int speed)
{
  if (speed >= 0)
  {
    if (index == 1)
    {
      digitalWrite(M_LEFT_1, 0);
      digitalWrite(M_LEFT_2, 1);
      analogWrite(M_LEFT_SPEED, speed);
    }
    else
    {
      digitalWrite(M_RIGHT_1, 1);
      digitalWrite(M_RIGHT_2, 0);
      analogWrite(M_RIGHT_SPEED, speed);
    }
  }
  else
  {
    if (index == 1)
    {
      digitalWrite(M_LEFT_1, 1);
      digitalWrite(M_LEFT_2, 0);
      analogWrite(M_LEFT_SPEED, -speed);

    }
    else
    {
      digitalWrite(M_RIGHT_1, 0);
      digitalWrite(M_RIGHT_2, 1);
      analogWrite(M_RIGHT_SPEED, -speed);
    }
  }
}


void getDistances(int* distance){
  distance[0] = uLeft.ping()/10;
  distance[1] = uFront.ping()/10;
  distance[2] = uRight.ping()/10;
}
int saveDistance[3]={};
float saveError = 0.0;
void PID(){
  getDistances(&distances[0]);
  float error0 = distances[0]-saveDistance[0];
  float error2 = distances[2]-saveDistance[2];
  error0/=50;
  error2/=50;

  float error = (abs(error0)+abs(error2))/2;
  if(error0<0){
    error*=-1;
  }
  error*=20;
  Serial.println(error);
  float lSpeed = STD_SPEED+LEFT_ADD_SPEED-(error*P)-((error-saveError)*D);
  float rSpeed = STD_SPEED+RIGHT_ADD_SPEED+(error*P)+((error-saveError)*D);
  if(lSpeed>255){
    lSpeed = 255;
  }else if(lSpeed < 100){
    lSpeed = 100;
  }
  if(rSpeed > 255){
    rSpeed = 255;
  }else if(rSpeed < 100){
    rSpeed = 100;
  }
  motor(1,(int)round(lSpeed));
  motor(0,(int)round(rSpeed));
  saveDistance[0] = distances[0];
  saveDistance[2] = distances[2];
  saveError = error;
  delay(100);
}

void moveForward(){
  motor(0,STD_SPEED);
  motor(1,STD_SPEED+LEFT_ADD_SPEED);
}

void moveBackward(){
  motor(0,-STD_SPEED);
  motor(1,-(STD_SPEED+LEFT_ADD_SPEED));
}

void turnLeft(){

}

void turnRight(){

}

void uTurn(){

}

void stop(int delayTime){

}