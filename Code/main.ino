#include <Servo.h>
#include <AFMotor.h>
#include <NewPing.h>

#define IR1 A0
#define IR A5
#define MAX_DISTANCE 300

#define Echo A0
#define Trig A1
#define motor 10
#define Speed 140
#define spoint 103

NewPing sonar(Trig, Echo, MAX_DISTANCE);

char value;
int distance;
int Left, Right, L, R, L1, R1;

Servo servo;
AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);

void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  servo.attach(motor);
  M1.setSpeed(Speed);
  M2.setSpeed(Speed);
  M3.setSpeed(Speed);
  M4.setSpeed(Speed);
}

void loop() {
  int IR_Sensor = digitalRead(IR);
  int IR_Sensor1 = digitalRead(IR1);
  voicecontrol();
}

void Bluetoothcontrol() {
  if (Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);
  }
  if (value == 'F') forward();
  else if (value == 'B') backward();
  else if (value == 'L') left();
  else if (value == 'R') right();
  else if (value == 'S') Stop();
}

void Obstacle(int IR_Sensor1) {
  if (IR_Sensor1 == 0) {
    Stop(); delay(200); backward(); delay(100); Stop();
    L = leftsee(); servo.write(spoint); delay(800); R = rightsee(); servo.write(spoint);
    if (L < R) { right(); delay(500); Stop(); delay(200); }
    else if (L > R) { left(); delay(500); Stop(); delay(200); }
  } else forward();
}

void voicecontrol() {
  if (Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);
    if (value == '^') {
      int IR_Sensor1 = digitalRead(IR1);
      while (IR_Sensor1 != 0) {
        IR_Sensor1 = digitalRead(IR1);
        if (IR_Sensor1 == 0) Stop();
        else forward();
        value = Serial.read();
        if (value != '^') { Stop(); break; }
      }
    } else if (value == '-') {
      int IR_Sensor = digitalRead(IR);
      while (IR_Sensor != 0) {
        IR_Sensor = digitalRead(IR);
        if (IR_Sensor == 0) Stop();
        else backward();
        value = Serial.read();
        if (value != '^') { Stop(); break; }
      }
    } else if (value == '<') {
      L = leftsee(); servo.write(spoint);
      if (L >= 0) { left(); delay(500); Stop(); } else if (L < 10) Stop();
    } else if (value == '>') {
      R = rightsee(); servo.write(spoint);
      if (R >= 0) { right(); delay(500); Stop(); } else if (R < 10) Stop();
    } else if (value == '*') Stop();
  }
}

int ultrasonic() {
  digitalWrite(Trig, LOW); delayMicroseconds(4);
  digitalWrite(Trig, HIGH); delayMicroseconds(10); digitalWrite(Trig, LOW);
  long t = pulseIn(Echo, HIGH); long cm = t / 29 / 2;
  return cm;
}

void forward() { M1.run(FORWARD); M2.run(FORWARD); M3.run(FORWARD); M4.run(FORWARD); }
void backward() { M1.run(BACKWARD); M2.run(BACKWARD); M3.run(BACKWARD); M4.run(BACKWARD); }
void right() { M1.run(BACKWARD); M4.run(BACKWARD); M3.run(FORWARD); M2.run(FORWARD); }
void left() { M1.run(FORWARD); M4.run(FORWARD); M3.run(BACKWARD); M2.run(BACKWARD); }
void Stop() { M1.run(RELEASE); M2.run(RELEASE); M3.run(RELEASE); M4.run(RELEASE); }

int rightsee() { servo.write(50); int IR_Sensor1 = digitalRead(IR1); delay(800); return IR_Sensor1; }
int leftsee() { servo.write(180); int IR_Sensor1 = digitalRead(IR1); delay(800); return IR_Sensor1; }
