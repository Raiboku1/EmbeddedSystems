// Include NewPing Library (ultrasonic~)
#include "NewPing.h"
#include <Servo.h>

// Hook up HC-SR04 with Trig to Arduino Pin 9, Echo to Arduino pin 10
Servo myservo;
int pos = 90;
#define TRIGGER_PIN 4
#define ECHO_PIN 3

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 5000

int distance = 0;
bool checkdist = false;
bool obstacleDetectedl = false;
bool obstacleDetectedr = false;

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Motor A connections (motors~)
int enA = 5;
int in1 = 12;
int in2 = 11;
// Motor B connections
int enB = 6;
int in3 = 10;
int in4 = 9;

#include <SoftwareSerial.h>
SoftwareSerial HM10(0, 1); // RX = 0, TX = 1
char appData;  
String inData = "";

void setup() {
  //ultrasonic
  Serial.begin(9600);
  myservo.attach(2);
  
	// Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);

  //bluetooth
  Serial.begin(9600);
  Serial.println("HM10 serial started at 9600");
  HM10.begin(9600); // set HM10 serial at 9600 baud rate
  pinMode(13, OUTPUT); // onboard LED
  digitalWrite(13, LOW); // switch OFF LED

}

void loop() {
  carcar();
}

// This function lets you control spinning direction of motors
void carfwd() {
	// Set motors to maximum speed
	// For PWM maximum possible values are 0 to 100
	analogWrite(enA, 100);
	analogWrite(enB, 100);

	// Turn on motor A & B
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
}

void carstop() {
	// Turn on motor A & B
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}

void turnleft(){
	analogWrite(enA, 100);
	analogWrite(enB, 100);

	// Turn on motor A & B
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
  delay(200);
}

void turnright(){
	analogWrite(enA, 100);
	analogWrite(enB, 100);

	// Turn on motor A & B
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  delay(200);
}

void turnreverse(){
	analogWrite(enA, 100);
	analogWrite(enB, 100);

	// Turn on motor A & B
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
  delay(1000);

	// Turn on motor A & B
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  delay(200);
}

int servoRh(){
      pos = 115;
      myservo.write(pos);
      delay(200);
      int distance3 = sonar.ping_cm();
      delay(50);
      return distance3;
}
int servoLh(){
      pos = 60;
      myservo.write(pos);
      delay(200);
      int distance2 = sonar.ping_cm();
      Serial.println(distance2);
      delay(50);
      return distance2;
}

int servoR(){
      carstop();
      pos = 160;
      myservo.write(pos);
      Serial.println("lookright");
      delay(500);
      int distance3 = sonar.ping_cm();
      delay(50);
      return distance3;
}
int servoL(){
      carstop();
      pos = 20;
      myservo.write(pos);
      Serial.println("look left");
      delay(500);
      int distance2 = sonar.ping_cm();
      Serial.println(distance2);
      delay(50);
      return distance2;
}
int servoF(){
      pos = 90;
      myservo.write(pos);
      Serial.println("lookfront");
      int distance1 = sonar.ping_cm();

      return distance1;
}

void carcar(){
  Serial.println(distance);
  distance = sonar.ping_cm();
  if (distance <= 40 && distance != 0){
    carstop();
    if(servoL()<= 40 && servoL() != 0){
      if(servoR()<= 40 && servoR() != 0){
        turnreverse();
      }
      else{
        turnleft();
      }
    }
    else{
      turnright();
    }
  }
  else{
    servoF();
    carfwd();
  }
  delay(400);
}