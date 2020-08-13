int ENA = 5, ENB = 6;
int IN1 = 7, IN2 = 8, IN3 = 9, IN4 = 11;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  calibrateStraight("forwards");
}

void loop() {
  // put your main code here, to run repeatedly:
}

void motorControl(int IN1State, int IN2State, int IN3State, int IN4State) {
  //digitalWrite(ENA, ENAState);
  //digitalWrite(ENB, ENBState);
  digitalWrite(IN1, IN1State);
  digitalWrite(IN2, IN2State);
  digitalWrite(IN3, IN3State);
  digitalWrite(IN4, IN4State);
}
void moveForwards(float distance) {
  motorControl(1, 0, 0, 1);
  timeDly = distance/ 2.45
  delay(timeDly);
  stopMoving();
}
void moveBackwards(float distance) {
  motorControl(0, 1, 1, 0);
  timeDly = distance/ 2.45
  delay(timeDly);
  stopMoving();
}
void stopMoving() {
  motorControl(0, 0, 0, 0);
}
void turn(float degree, String dir) {
  if (dir == "left") {
    motorControl(0, 1, 0, 1);
  }
  else if (dir == "right") {
    motorControl(1, 0, 1, 0);
  }
  delay(distance);
  stopMoving();
}
void calibrateStraight(String dir) {
  if (dir == "forwards") {
    motorControl(1, 0, 0, 1);
  }
  else if ( dir == "backwards") {
    motorControl(0, 1, 1, 0);
  }
  delay(5000); // goes 12 feet and 3 inches in 5 seconds 12.25 = rate * time.
  //r = d/t . r = 12.25/5 = 2.45 feet/second. t = d/r. t = d/ 2.45. t = d/2.45
  stopMoving();
}
void calibrateTurn(String dir) {
  if (dir == "left") {
    motorControl(0, 1, 0, 1);
  }
  else if (dir == "right") {
    motorControl(1, 0, 1, 0);
  }
  delay(5000);
  stopMoving();
}
