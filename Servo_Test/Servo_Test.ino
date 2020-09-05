#include <Servo.h>
int serPin = 9;
int serPin2 = 10;
int x = 0;
Servo ser;
Servo ser2;
void setup() {
  // put your setup code here, to run once:
  ser.attach(serPin);
  ser2.attach(serPin2);
  ser.write(180);
  ser2.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*for (int j = 0; j<=180; j++) {
    ser.write(j);
    ser2.write(j);
    delay(20);
  }*/
  
}
