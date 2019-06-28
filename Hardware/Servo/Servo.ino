#include<Servo.h>
#include <ESP8266WiFi.h>
Servo MyServo1, MyServo2;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  // define pinMode Servo
  MyServo1.attach(D1);
  MyServo1.write(0);
  delay(1000);
  MyServo2.attach(D2);
  MyServo2.write(180);

}

void loop() {
  // put your main code here, to run repeatedly:
  MyServo1.attach(D1);
  MyServo1.write(22);
  delay(1000);
  MyServo1.attach(D1);
  MyServo1.write(0);
  delay(1000);
  MyServo2.attach(D2);
  MyServo2.write(180);
  delay(1000);
  MyServo2.attach(D2);
  MyServo2.write(155);
  delay(1000);
  MyServo2.attach(D2);
  MyServo2.write(180);
  delay(1000);

}
