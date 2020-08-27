#include <PS4BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

int ENA = 2, ENB = 8;
int IN1 = 3, IN2 = 4, IN3 = 5, IN4 = 6;
float leftSpeed, rightSpeed, minSpeed = 100, maxSpeed = 255, speedVal, straightSpeed;
float timeDly, baseFeetPerSecond = 2.2, baseDegreesPerSecond = 360, degreesPerSecond;
int RedLEDsPin = 48, BlueLEDsPin = 53;
int L2ButtonVal, R2ButtonVal, L3StickXVal;
int straightVelocity, turnSpeed;
void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(RedLEDsPin, OUTPUT);
  pinMode(BlueLEDsPin, OUTPUT);
  ledAction("Red", 0);
  ledAction("Blue", 0);
  //digitalWrite(ENA, HIGH);
  //digitalWrite(ENB, HIGH);
  //runCommand();
}

void loop() {
  Usb.Task();

  if (PS4.connected()) {
    if (PS4.getButtonClick(L3)) { // checkRedTeam(); add functions for if statements.
      Serial.print(F("\r\nL3\tRed Team"));
      PS4.setLed(Red);
      ledAction("Red", 1);
      ledAction("Blue", 0);
    }
    if (PS4.getButtonClick(R3)) {
      Serial.print(F("\r\nR3\tBlue Team"));
      PS4.setLed(Blue);
      ledAction("Red", 0);
      ledAction("Blue", 1);
    }
    if (PS4.getAnalogButton(L2)) {
      L2ButtonVal = PS4.getAnalogButton(L2);
      if (L2ButtonVal <= 100) {
        Serial.print(F("\r\nStopping"));
        stayStopped();
      }
      if (L2ButtonVal != oldL2Value && L2ButtonVal >= 100) {
        Serial.print(F("\r\nL2: "));
        Serial.print(L2ButtonVal);
        //Serial.print(F("\r\tStraightVelocity: "));
        Serial.print(F("\r\tReversing"));
        //straightVelocity = map(L2ButtonVal, 0 , 255, minSpeed, maxSpeed);
        //Serial.print(straightVelocity);
        continueBackwards(L2ButtonVal);
      }
    }
    if (PS4.getAnalogButton(R2)) {
      R2ButtonVal = PS4.getAnalogButton(R2);
      if (R2ButtonVal <= 100) {
        Serial.print(F("\r\nStopping"));
        stayStopped();
      }
      if (R2ButtonVal != oldR2Value && R2ButtonVal >= 100) {
        Serial.print(F("\r\nR2: "));
        Serial.print(R2ButtonVal);
        //Serial.print(F("\r\tStraightVelocity: "));
        Serial.print(F("\r\tGoing Forwards"));
        //straightVelocity = map(L2ButtonVal, 0 , 255, minSpeed, maxSpeed);
        //Serial.print(straightVelocity);
        continueForwards(R2ButtonVal);
      }
    }
    oldL2Value = PS4.getAnalogButton(L2);
    oldR2Value = PS4.getAnalogButton(R2);

    /*if (PS4.getAnalogHat(LeftHatX) < 137 && PS4.getAnalogHat(LeftHatX) > 117) {
      //Serial.print(F("\r\nStopping"));
      //stayStopped();
    }*/
    if (PS4.getAnalogHat(LeftHatX) < 117) {
      L3StickXVal = PS4.getAnalogHat(LeftHatX);
      turnSpeed = map(L3StickXVal, 117, 0, minSpeed, maxSpeed);
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(L3StickXVal);
      Serial.print(F("\r\tTurning Left "));
      //continueTurning("left");
      continueTurningWithSpeed("left", turnSpeed);
    }
    else if (PS4.getAnalogHat(LeftHatX) > 137) {
      L3StickXVal = PS4.getAnalogHat(LeftHatX);
      turnSpeed = map(L3StickXVal, 137, 255, minSpeed, maxSpeed);
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(L3StickXVal);
      Serial.print(F("\r\tTurning Right "));
      continueTurningWithSpeed("right", turnSpeed);
      //continueTurning("right");
    }
    else {
      //Serial.print(F("\r\nStopping"));
      //stayStopped();
    }


    if (PS4.getButtonClick(L1)) {
      Serial.print(F("\r\nL1\tStopping"));
      stayStopped();
    }

  }

}

void ledAction(String color, int ledState) {
  if (color == "Red") {
    digitalWrite(RedLEDsPin, ledState);
  }
  if (color == "Blue") {
    digitalWrite(BlueLEDsPin, ledState);
  }
}
void motorControl(int IN1State, int IN2State, int IN3State, int IN4State) {
  //digitalWrite(ENA, ENAState);
  //digitalWrite(ENB, ENBState); // look up math for map function use map but not function. only math equation.
  digitalWrite(IN1, IN1State);
  digitalWrite(IN2, IN2State);
  digitalWrite(IN3, IN3State);
  digitalWrite(IN4, IN4State);
}
void setSpeed(int ENAVal, int ENBVal) {
  analogWrite(ENA, ENAVal);
  analogWrite(ENB, ENBVal);
  leftSpeed = ENAVal;
  rightSpeed = ENBVal;
  straightSpeed = ENAVal;

}
void stopMoving() {
  //setSpeed(0,0);
  motorControl(0, 0, 0, 0);
}
void stayStopped() {
  setSpeed(0, 0);
  stopMoving();
}
void continueForwards(int motorSpeed) {
  setSpeed(motorSpeed, motorSpeed);
  motorControl(1, 0, 0, 1);
}
void continueBackwards(int motorSpeed) {
  setSpeed(motorSpeed, motorSpeed);
  motorControl(0, 1, 1, 0);
}
void continueTurning(String dir) {
  setSpeed(125, 125);
  if (dir == "left") {
    motorControl(0, 1, 0, 1);
  }
  else if (dir == "right") {
    motorControl(1, 0, 1, 0);
  }
}
void continueTurningWithSpeed(String dir, int turnVal) {
  setSpeed(turnVal, turnVal);
  if (dir == "left") {
    motorControl(0, 1, 0, 1);
  }
  else if (dir == "right") {
    motorControl(1, 0, 1, 0);
  }
}
void moveForwards(float distance, float feetPerSecond) {
  //feetPerSecond = baseFeetPerSecond * (straightSpeed / maxSpeed);
  //feetPerSecond = (.0075 * straightSpeed) + .35;
  speedVal = (feetPerSecond - 0.35) / 0.0075;
  timeDly = (distance / feetPerSecond) * 1000.0;
  setSpeed(speedVal, speedVal);
  Serial.print(speedVal);
  Serial.print("\t\t");
  Serial.println(timeDly);
  motorControl(1, 0, 0, 1);
  delay(timeDly);
  stopMoving();
}
void moveBackwards(float distance, float feetPerSecond) {
  //feetPerSecond = baseFeetPerSecond * (straightSpeed / maxSpeed);
  speedVal = (feetPerSecond - 0.35) / 0.0075;
  timeDly = (distance / feetPerSecond) * 1000.0;
  setSpeed(speedVal, speedVal);
  motorControl(0, 1, 1, 0);
  delay(timeDly);
  stopMoving();
}
void turn(int degree, String dir) {
  stopMoving();
  delay(100);
  setSpeed(125, 125); // add var
  if (dir == "left") {
    motorControl(0, 1, 0, 1);
    //degreesPerSecond = map(leftSpeed, minSpeed, maxSpeed,
    //degreesPerSecond = baseDegreesPerSecond * (leftSpeed / maxSpeed);
  }
  else if (dir == "right") {
    motorControl(1, 0, 1, 0);
    //degreesPerSecond = baseDegreesPerSecond * (rightSpeed / maxSpeed);
  }
  // Omega = 398 degrees per second.
  // Theta = Omega * time

  //timeDly = (degree / degreesPerSecond) * 1000;
  timeDly = (degree + 6) / 136.29 * 1000;
  delay(timeDly);
  stopMoving();
}
void calibrateStraight(String dir) {
  if (dir == "forwards") {
    motorControl(1, 0, 0, 1);
  }
  else if ( dir == "backwards") {
    motorControl(0, 1, 1, 0);
  }
  delay(1000); // goes 12 feet and 3 inches in 5 seconds 12.25 = rate * time.
  //r = d/t . r = 12.25/5 = 2.45 feet/second. t = d/r. t = d/ 2.45. t = d/2.45
  stopMoving();
}
void calibrateTurn(String dir) {
  stopMoving();
  setSpeed(125, 125);
  if (dir == "left") {
    motorControl(0, 1, 0, 1);
  }
  else if (dir == "right") {
    motorControl(1, 0, 1, 0);
  }
  delay(5000); //5 Revolutions and 190 degrees.  360*5 + 190. 1800 + 190 = 1990
  // Theta = Omega * time. Omega = Theta/time. Omega = 1990/5. Omega = 398 degrees per second.
  //speedVal = (feetPerSecond - 0.35) / 0.0075;
  //setSpeed(speedVal, speedVal);
  stopMoving();
}
void runCommand() {
  moveForwards(3, 1.3);
  turn(90, "right");
  moveForwards(1.5, 2);
  turn(270, "left");
  moveBackwards(3, 1.9);
  turn(360, "right");
}
