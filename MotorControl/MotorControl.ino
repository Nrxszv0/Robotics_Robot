int ENA = 5, ENB = 6;
int IN1 = 7, IN2 = 8, IN3 = 9, IN4 = 11;
void setup() {
  // put your setup code here, to run once:
pinMode(ENA, OUTPUT);
pinMode(ENB, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
digitalWrite(ENA, HIGH);
digitalWrite(ENB, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(IN1, HIGH);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
delay(1000);
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);

}
