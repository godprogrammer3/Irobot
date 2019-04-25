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
#define E_RIGHT 13
#define E_Left 12
#define STD_SPEED 100
#define LEFT_ADD_SPEED 0
#define RIGHT_ADD_SPEED 10

#define MAX_DISTANCE 200
#define P 12
#define I 0
#define D 5
NewPing uRight(U_RIGHT_TRIG, U_RIGHT_ECHO, MAX_DISTANCE);
NewPing uLeft(U_LEFT_TRIG, U_LEFT_ECHO, MAX_DISTANCE);
NewPing uFront(U_FRONT_TRIG, U_FRONT_ECHO, MAX_DISTANCE);

int distances[3] = {};

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

void getDistances(int *distance)
{
  distance[0] = uLeft.ping() / 10;
  distance[1] = uFront.ping() / 10;
  distance[2] = uRight.ping() / 10;
}
int saveDistance[3] = {};
float saveError = 0.0;
void PID()
{

  getDistances(&distances[0]);

  float error;
  if (abs(distances[0] - saveDistance[0]) > 2 && abs(distances[2] - saveDistance[2]) > 2)
  {
    float error0 = distances[0] - saveDistance[0];
    float error2 = distances[2] - saveDistance[2];
    error0 /= 15;
    error2 /= 15;
    if (distances[0] == 0 || distances[0] > 100)
      error = abs(error2);
    else if (distances[2] == 0 || distances[2] > 100)
      error = abs(error0);
    else
      error = (abs(error0) + abs(error2)) / 2;

    if (error0 < 0)
    {
      error *= -1;
    }
  }
  else
  {
    error = 0;
  }

  Serial.println(error);
  float lSpeed = STD_SPEED + LEFT_ADD_SPEED - (error * P) - ((error - saveError) * D);
  float rSpeed = STD_SPEED + RIGHT_ADD_SPEED + (error * P) + ((error - saveError) * D);
  if (lSpeed > 255)
  {
    lSpeed = 255;
  }
  else if (lSpeed < 80)
  {
    lSpeed = 80;
  }
  if (rSpeed > 255)
  {
    rSpeed = 255;
  }
  else if (rSpeed < 80)
  {
    rSpeed = 80;
  }
  motor(1, (int)round(lSpeed));
  motor(0, (int)round(rSpeed));
  saveDistance[0] = distances[0];
  saveDistance[2] = distances[2];
  saveError = error;
  delay(50);
}

void PID2()
{

  getDistances(&distances[0]);

  float error;
  if (distances[0] < 300 && distances[0] > 5)
  {
    error = (distances[0] - 300) / 100.0;
  }
  else if (distances[2] < 300 && distances[2] > 5)
  {
    error = -(distances[2] - 300) / 100.0;
  }
  else{error = 0;}
  Serial.println(error);
  float lSpeed = STD_SPEED + LEFT_ADD_SPEED - (error * P) - ((error - saveError) * D);
  float rSpeed = STD_SPEED + RIGHT_ADD_SPEED + (error * P) + ((error - saveError) * D);
  if (lSpeed > 255)
  {
    lSpeed = 255;
  }
  else if (lSpeed < 80)
  {
    lSpeed = 80;
  }
  if (rSpeed > 255)
  {
    rSpeed = 255;
  }
  else if (rSpeed < 80)
  {
    rSpeed = 80;
  }
  motor(1, (int)round(lSpeed));
  motor(0, (int)round(rSpeed));
  saveError = error;
}

void moveForward()
{

  motor(0, STD_SPEED + RIGHT_ADD_SPEED);
  motor(1, (STD_SPEED + LEFT_ADD_SPEED));
}
void moveBackward()
{
  motor(0, -(STD_SPEED + RIGHT_ADD_SPEED));
  motor(1, -(STD_SPEED + LEFT_ADD_SPEED));
}

void turnLeft(float R){
  int lastRight, lastLeft, countRight = 0, countLeft = 0;
  bool checkR = true, checkL = true;
  while (countRight / 2.0 < R && countLeft / 2.0 < R)
  {
    int right = digitalRead(E_RIGHT), left = digitalRead(E_Left);

    if (checkR)
      lastRight = right, checkR = false;
    if (checkL)
      lastLeft = left, checkL = false;

    if (right != lastRight)
    {
      countRight++;
    }
    if (left != lastLeft)
    {
      countLeft++;
    }

    motor(0, STD_SPEED + RIGHT_ADD_SPEED);
    motor(1, -(STD_SPEED + LEFT_ADD_SPEED));

    lastLeft = left;
    lastRight = right;
  }
  motor(0, -(STD_SPEED + RIGHT_ADD_SPEED));
  motor(1, (STD_SPEED + LEFT_ADD_SPEED));

  delay(76);
  motor(0, 0);
  motor(1, 0);

}
void turnRight(float R)
{
  int lastRight, lastLeft, countRight = 0, countLeft = 0;
  bool checkR = true, checkL = true;
  while (countRight / 2.0 < R && countLeft / 2.0 < R)
  {
    int right = digitalRead(E_RIGHT), left = digitalRead(E_Left);

    if (checkR)
      lastRight = right, checkR = false;
    if (checkL)
      lastLeft = left, checkL = false;

    if (right != lastRight)
    {
      countRight++;
    }
    if (left != lastLeft)
    {
      countLeft++;
    }

    motor(0, -(STD_SPEED + RIGHT_ADD_SPEED));
    motor(1, (STD_SPEED + LEFT_ADD_SPEED));

    lastLeft = left;
    lastRight = right;
  }
  motor(0, (STD_SPEED + RIGHT_ADD_SPEED));
  motor(1, -(STD_SPEED + LEFT_ADD_SPEED));

  delay(76);
  motor(0, 0);
  motor(1, 0);
}

void uTurn()
{
}

void stopZero()
{
  motor(0, 0);
  motor(1, 0);
}
void stopForward(int delayTime)
{
  motor(0, -90);
  motor(1, -90);
  delay(65);
  stopZero();
  delay(delayTime);
}
void stopBackward(int delayTime)
{
  motor(0, 90);
  motor(1, 90);
  delay(65);
  stopZero();
  delay(delayTime);
}

void setup()
{

  pinMode(M_RIGHT_1, OUTPUT);
  pinMode(M_RIGHT_2, OUTPUT);
  pinMode(M_LEFT_1, OUTPUT);
  pinMode(M_LEFT_2, OUTPUT);
  pinMode(M_RIGHT_SPEED, OUTPUT);
  pinMode(M_LEFT_SPEED, OUTPUT);

  pinMode(E_Left, INPUT);
  pinMode(E_RIGHT, INPUT);
/*-----------------------------------------------------------------------------------------------------*/
  
  //Serial.begin(9600);

/*---------------------------------------------------------------------------------------------------*/
}
int currentState = 0, count = 0;
void loop()
{
  getDistances(&distances[0]);
  
  switch (currentState)
  {
    case -1:
    {
      Serial.print(distances[0]);
      Serial.print(",");
      Serial.print(distances[1]);
      Serial.print(",");
      Serial.print(distances[2]);
      Serial.println("        ");
      break;
    }
    case -2:
    {
      PID2();
      break;
    }
    case -3:
    {
      moveForward();
      break;
    }
    case -4:
    {
      turnLeft(9);
      delay(1000);
      currentState = -5;
      break;
    }
    case -5: 
    {
      turnRight(9);
      delay(1000);
      currentState = -4;
      break;
    }




    /*----------------------------------------------------------------------------------------------------------*/
    case 0:
    {
      PID2();
      if (distances[1] < 180)
      {
        moveForward();
        if(distances[1] < 40){
          stopForward(1000);
          currentState++;
        }
      }
      else PID2();
      break;
    }
    case 1:
    {
      turnLeft(8.5);
      delay(500);
      currentState++;
      break;
     
    }
    case 2:
    {
      break;
    }
  }
}