# Wifi and IR robot Project
	
  The Wifi and IR robot is a system of connected parts in which a robot body is controlled from a basic controller app or an IR remote. A main cardboard box body holds an Arduino Uno wired to different components for the different functions that the robot can make.
- For the motor functions the Arduino Uno is wired through an L293D motor driver to two DC motors with attached wheels, these can be control through the IR remote and the boxbot-controller app. 
- For the headlights function a light sensor is wired to an analog pin from which the information about the amount of light in the environment is received. There are also two jumbo LEDs connected with resistors to the Arduino’s digital pins and attached in front of the cardboard Box, these LEDs turn on when the ambient light is low.
- For the turning signals two LEDs are connected to digital pins and attached to each side of the robot, they can be controlled with IR signals or the boxbot-controller App.
- For the backlight functions a single LED wired to the a digital pin is attached to the back of the robot and turns on when the robot stops moving.
- For the waving arm function a servo motor is connected to a digital pin on the arduino and responds to commands based on IR information and can also be controlled through the boxbot-controller App.
Wired there are two ways to control the robot’s functions one is by using an infrared TV remote  to send signals to the IR receiver wired with a capacitor to a digital pin on the Arduino board. The other way to control the robot is through the boxbot-controller App which would send signals through Wifi connection to a Raspberry Pi board connected to the Arduino Uno through a USB-b cable.

  For the software, there are two main sketches utilized. The first sketch is written in C++ and is compiled by the Arduino App and uploaded directly from to the Arduino board. This program includes all the functions the robot can do and a series of commands to make the robot do the different actions based on the information provided. If the information are infrared signals, the code responds to the IR hex numbers. If the information is through the boxbot-App, the code reads the information given through the Raspberry Pi. This code is stored in the arduino-example folder.

```javascript
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

    if (input == "forward") {
      Serial.println("FORWARD");
      forward();
    } else if (input == "reverse") {
      Serial.println("REVERSE");
      reverse();
    } else if (input == "left") {
      Serial.println("LEFT");
      left();
    } else if (input == "right") {
      Serial.println("RIGHT");
      right();
    } else if (input == "halt") {
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

```

  The second code made for the project is made with React and it sets up an App where the user can press a button and depending on the button it will trigger a message that will be sent to the Raspberry Pi through wifi and then to the Arduino board where the message will be interpreted by the firs sketch. This code is on the Client.js folder
  
```javascript
class ControlComponent extends React.Component {
  constructor(props) {
    super(props);

    // make a websocket connection to the server we loaded this page from.
    this.socket = new WebSocket(`ws://${window.location.host}/comm`);

    // when the socket closes, issue an alert.
    this.socket.addEventListener('close', () => {
      alert("Socket connection to server closed.");
    });

    // when there's a message from the server, use the handleMessage function
    // to handle it.
    this.socket.addEventListener('message', message => {
      this.handleMessage(message);
    })
  }

  handleMessage(message) {
    console.log("Message:", message.data);
    // do something with this data?
  }

  sendMessage(message) {
    // send the message to the server over the websocket.
    this.socket.send(message);
  }

  render() {
    // four buttons. see index.html for styling.
    return <div className="wrapper">
        <button onClick={() => this.sendMessage("forward")}>Fwd</button><br />
        <button onClick={() => this.sendMessage("left")}>Left</button>
        <button onClick={() => this.sendMessage("right")}>Right</button><br />
        <button onClick={() => this.sendMessage("reverse")}>Back</button><br />
        <button onClick={() => this.sendMessage("halt")}>Halt</button>< br/>
        <button onClick={() => this.sendMessage("wave")}>Wave</button><br />
        <button onClick={() => this.sendMessage("leftSig")}>Left Signal</button>
        <button onClick={() => this.sendMessage("rightSig")}>Right Signal</button>
      </div>
  }
}

// render that control component, defined above, into the "root" element of index.html
ReactDOM.render(
  <ControlComponent />,
  document.getElementById('root')
);
```


