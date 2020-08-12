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
  moveForwards();
  delay(2000);
  stopMoving();
  delay(400);
  turn(550, "right");
  moveForwards();
  delay(2000);
  stopMoving();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void motorControl(int ENAState, int ENBState, int IN1State, int IN2State, int IN3State, int IN4State) {
  digitalWrite(ENA, ENAState);
  digitalWrite(ENB, ENBState);
  digitalWrite(IN1, IN1State);
  digitalWrite(IN2, IN2State);
  digitalWrite(IN3, IN3State);
  digitalWrite(IN4, IN4State);
}
void moveForwards() {
  motorControl(1, 1, 1, 0, 0, 1);
}
void moveBackwards() {
  motorControl(1, 1, 0, 1, 1, 0);
}
void stopMoving() {
  motorControl(0, 0, 0, 0, 0, 0);
}
void turn(int duration, String dir) {
  if (dir == "left") {
    motorControl(1, 1, 0, 1, 0, 1);
    delay(duration);
  }
  if (dir == "right") {
    motorControl(1, 1, 1, 0, 1, 0);
    delay(duration);
  }
  stopMoving();
}
