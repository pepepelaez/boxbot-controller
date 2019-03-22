#include <Servo.h>

/**
  Remote Control Motor Driver

  Set two motor pins based on input from an infrared remote control.
 **/

#include <IRremote.h>

int servoCharPin = 3;
Servo servoChar;
int remoteInputPin = 2;
IRrecv receiver(remoteInputPin);
decode_results results;

int motorRightForward   = 9;
int motorRightReverse   = 10;
int motorLeftForward    = 11;
int motorLeftReverse    = 12;
int ledR = 5;
int ledL = 6;
int blinkR = false;
int blinkL = false;
int automaticBreakLight = 7;
int headlight1 = 8;
int headlight2 = 13;
int lightValue;

void setup() {
  Serial.begin(115200);
  receiver.enableIRIn();

  pinMode(motorRightForward, OUTPUT);
  pinMode(motorRightReverse, OUTPUT);
  pinMode(motorLeftForward, OUTPUT);
  pinMode(motorLeftReverse, OUTPUT);

  servoChar.attach(servoCharPin);

  pinMode(automaticBreakLight, OUTPUT);
  pinMode(headlight1, OUTPUT);
  pinMode(headlight2, OUTPUT);
  pinMode(A0, INPUT);
}

void forward() {
  digitalWrite(automaticBreakLight, LOW);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightReverse, LOW);
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftReverse, LOW);
}

void reverse() {
  digitalWrite(automaticBreakLight, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightReverse, HIGH);
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftReverse, HIGH);
}

void left() {
  digitalWrite(automaticBreakLight, LOW);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightReverse, LOW);
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftReverse, HIGH);
}

void right() {
  digitalWrite(automaticBreakLight, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightReverse, HIGH);
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftReverse, LOW);
}

void halt() {
  digitalWrite(automaticBreakLight, HIGH);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightReverse, LOW);
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftReverse, LOW);
}

void wave(){
  servoChar.write(0);
  delay(300);
  servoChar.write(180);
  delay(300);
  servoChar.write(0);
  delay(300);
  servoChar.write(180);
  delay(300);
  servoChar.write(90);
  delay(300);  
}

void loop() {
  if (receiver.decode(&results)) {
    Serial.println(results.value, HEX);

    if (results.value == 0x61) {
      Serial.println("FORWARD");
      forward();
    } else if (results.value == 0xCB9C) {
      Serial.println("REVERSE");
      reverse();
    } else if (results.value == 0x4D1) {
      Serial.println("LEFT");
      left();
    } else if (results.value == 0x1CB9C) {
      Serial.println("RIGHT");
      right();
    } else if (results.value == 0x9CB9C) {
      Serial.println("HALT");
      halt();
    }
    else if (results.value == 0x146) { 
      Serial.println("WAVE");
      wave();
    }
    else if (results.value == 0x8CB9C) {
      if (blinkR == false){ 
        Serial.println("RIGHT TURN");
        blinkL = false;
        blinkR = true;
      }
      else {
        blinkR = false;
      }
    }
    else if (results.value == 0xF16) { 
            if (blinkL == false){ 
        Serial.println("LEFT TURN");
        blinkR = false;
        blinkL = true;
      }
      else {
        blinkL = false;
      }
    }

    receiver.resume();
  }
  if (blinkR == true){
    
    analogWrite(ledR, 255);
    delay(300);  
    analogWrite(ledR, 0);
    delay(300);
  }
  else{
    analogWrite(ledR, 0);
  }
  if (blinkL == true){
    analogWrite(ledL, 255);
    delay(300);  
    analogWrite(ledL, 0);
    delay(300);
  }
  else{
    analogWrite(ledL, 0);
  }
  
  lightValue = analogRead(A0);
  Serial.print(lightValue);
  Serial.println(" ");
  if (lightValue>700) {
    Serial.println("Too Dark, Lights on");
    digitalWrite(headlight1, HIGH);
    digitalWrite(headlight2, HIGH);
  }
  else {
    digitalWrite(headlight1, LOW);
    digitalWrite(headlight2, LOW);
  }
}

void serialEvent(){
  String input = Serial.readStringUntil('\n');
  input.trim();
  
//    if (receiver.decode(&results)) {
//    Serial.println(results.value, HEX);

    if (input == "forward") {
      Serial.println("FORWARD");
      forward();
    } else if (input == "backwards") {
      Serial.println("REVERSE");
      reverse();
    } else if (input == "left") {
      Serial.println("LEFT");
      left();
    } else if (input == "right") {
      Serial.println("RIGHT");
      right();
    } else if (results.value == 0x9CB9C) {
      Serial.println("HALT");
      halt();
    }
    else if (input == "wave") { 
      Serial.println("WAVE");
      wave();
    }
    else if (input == "rightSig") {
      if (blinkR == false){ 
        Serial.println("RIGHT TURN");
        blinkL = false;
        blinkR = true;
      }
      else {
        blinkR = false;
      }
    }
    else if (input == "leftSig") { 
            if (blinkL == false){ 
        Serial.println("LEFT TURN");
        blinkR = false;
        blinkL = true;
      }
      else {
        blinkL = false;
      }
    }
//
//    receiver.resume();
//  }
  if (blinkR == true){
    
    analogWrite(ledR, 255);
    delay(300);  
    analogWrite(ledR, 0);
    delay(300);
  }
  else{
    analogWrite(ledR, 0);
  }
  if (blinkL == true){
    analogWrite(ledL, 255);
    delay(300);  
    analogWrite(ledL, 0);
    delay(300);
  }
  else{
    analogWrite(ledL, 0);
  }
}
