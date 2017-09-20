/**Line maze solver
 * Jacob Regenstein
 * 
 * Uses left turn rule
 * 
 * 
 */

#include "MeOrion.h"
//motor1 is right motor, motor 2 is left motor. I'd like to change this but it works as is and I don't want to introduce bugs
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeBuzzer buzzer;

#define NOTE_A4 440

uint8_t motorSpeed = 100;

//Default linear speed, in cm/s
double linearSpeed = 10.0;

//Default angular speed, in deg/s
double angularSpeed = 120.0;

//Scaling factor to convert the motor speed, in cm/s, to a value to send to controllers
double motSpeedLinearScalar = 7.5;

//scaling factor to convert angular rate, in deg/s, to a value to send the controllers
double motSpeedAngularScalar = .65;

//light sensors numbered consecutively, from left to right
MeLightSensor lightSensor1(PORT_1);
MeLightSensor lightSensor2(PORT_6);
MeLightSensor lightSensor3(PORT_5);
MeLightSensor lightSensor4(PORT_2);

//stops all motors
void stop(){
  motor1.stop();
  motor2.stop();
}

//moves the vehicle forward or backwards at the specified speed, in cm/s
void setLinearVel(double vel){
  if(0==vel){
    stop();
  }else{
    motor1.run((int)(vel*motSpeedLinearScalar));
    motor2.run(-(int)(vel*motSpeedLinearScalar));
  }
}

//sets linear forward speed, with a bias to one side or the other. Positive bias causes a curve to the left
void setLinearVel(double vel, double bias){
  if(0==vel){
    stop();
  }else{
    motor1.run((int)((vel + bias)*motSpeedLinearScalar));
    motor2.run(-(int)((vel - bias)*motSpeedLinearScalar));
  } 
}

//turns the vehicle at the specified number of degrees per second. Left turn is positive
void setAngularVel(double vel){
  if(0==vel){
    stop();
  }else{
    motor1.run((int)(vel*motSpeedAngularScalar));
    Serial.println((int)(vel*motSpeedAngularScalar));
    motor2.run((int)(vel*motSpeedAngularScalar));
  }
}

//moves forward the specified number of cm
void move(double x){
  stop();
  setLinearVel(linearSpeed);
  delay((int)(1000.0*(x / linearSpeed)));
  stop();
  delay(100);
}

//turns by a given angle, in degrees
void turn(double theta){
  stop();
  if(theta>0){
    setAngularVel(angularSpeed);
  }else if(theta<0){
    setAngularVel(-angularSpeed);
  }
  delay(abs((1000.0*(theta / angularSpeed))));
  stop();
  delay(100);
}

//returns true if sensor 1 (leftmost sensor) detects a white surface. The following functions do the same, sensors are numbered sequentially left to right.
boolean sensor1(){
  return lightSensor1.read() > 500;
}
boolean sensor2(){
  return lightSensor2.read() > 500;
}
boolean sensor3(){
  return lightSensor3.read() > 500;
}
boolean sensor4(){
  return lightSensor4.read() > 500;
}

//keeps the robot on the line. calls evaluateIntersection when it reaches one. It requires lines that are thicker than the two sensors, otherwise it will think it's hit a dead end
void lineFollow(){
  setLinearVel(10);
  //go until we reach an intersection
  while(sensor1() && sensor4()){
    //if we're off to the left turn right
    if(!sensor3()&& sensor2()){
      setLinearVel(10,-5);
      while(!sensor3()&& sensor2()){
        //delay(5);
      }
      setLinearVel(10);
    }
    //if we're off to the right turn left
    if(!sensor2()&& sensor3()){
      setLinearVel(10,5);
      while(!sensor2()&& sensor3()){
        //delay(5);
      }
      setLinearVel(10);
    }
    if(sensor2() && sensor3()){
      uTurn();
      setLinearVel(10);
    }
    delay(10);
  }
  //call evaluateIntersection()
  evaluateIntersection();
}

//called when we get to an intersection
void evaluateIntersection(){
  //move farward just a bit so we can make sure the sensors are over the intersection
  move(0.5);
  boolean backleft = !sensor1();
  boolean backright = !sensor4();
  //move forward so we can either turn onto lines or detect if we're at the goal.
  move(2);
  boolean frontleft = !sensor1();
  boolean frontright = !sensor4();
  //check if we've reached the goal
  if(frontleft && frontright) celebrate();
  //if we can turn left, turn left
  move(6);
  if(backleft){
    left();
    lineFollow();
  //if we can't turn left decide if we should turn right or straight
  }else{
    //if we can go straight, go straight
    if(!sensor2() || !sensor3()){
      lineFollow();
    //if not turn right
    }else{
      right();
      lineFollow();
    }
  }
}

//turns to the left, then calls lineFollow
void left(){
  //get off the line
  turn(80);
  setAngularVel(angularSpeed);
  //turn until we see the next line
  while(sensor2() || sensor3()){
    //delay(10);
  }
  stop();
  lineFollow();
}

//turns to the right, then calls lineFollow
void right(){
  //get off the line
  turn(-80);
  setAngularVel(-angularSpeed);
  //turn until we see the next line
  while(sensor2() || sensor3()){
    //delay(10);
  }
  stop();
  lineFollow();
}

//turns around, stops on line. Called within lineFollow
void uTurn(){
  move(8);
  turn(160);
  setAngularVel(angularSpeed);
  while(sensor2() || sensor3()){
    //delay(10);
  }
  stop();
}



//behavior when reach goal
void celebrate(){
    buzzer.tone(NOTE_A4,beeptime);
    delay(100);
    buzzer.noTone();
  //We don't want this loop to end, otherwise the loop function will call lineFollow() again
  while(true){
    turn(90);
    turn(-90);
    turn(-45);
    turn(-45);
    turn(90);
  }
}

void setup() {
  // put your setup code here, to run once:
  // initialize serial communications at 9600 bps
  Serial.begin(9600);
}

void loop() {
  //don't actually need to loop this, but it doesn't really matter. Linefollow will never terminate.
  lineFollow();
}



