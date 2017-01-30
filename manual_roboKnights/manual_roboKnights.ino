#include <hid.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include<Servo.h>
#include "hidjoystickrptparser.h"
void(*re_start)(void)=0;
Servo grip,wrist;
USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
JoystickEvents JoyEvents;
JoystickReportParser Joy(&JoyEvents);
int base_speed = 130;
int grip_con = 0;
int wrist_con = 0;
int grip_pin = 7   ;
int wrist_pin = 6;
int motor_l1 = 2;
int motor_l2 = 3;
int motor_r1 = 4;
int motor_r2 = 5;
int ball_shooter = 22;
int bz_pin = 22;
int bz_gnd = 28;
void setup() {
  pinMode(bz_pin,OUTPUT);
  pinMode(bz_gnd,OUTPUT);
  digitalWrite(bz_gnd,LOW);
  grip.attach(grip_pin);
  wrist.attach(wrist_pin);
  grip_con = 110;
  wrist_con = 107;
  pinMode(ball_shooter,OUTPUT);
  pinMode(motor_l1,OUTPUT);
  pinMode(motor_l2,OUTPUT);
  pinMode(motor_r1,OUTPUT);
  pinMode(motor_r2,OUTPUT);
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay(200);

  if (!Hid.SetReportParser(0, &Joy))
    ErrorMessage<uint8_t > (PSTR("SetReportParser"), 1);
}

void loop() {
  Usb.Task();
  grip.write(grip_con);
  wrist.write(wrist_con);
  switch(JoyEvents.button)
  {
  case 0:
    {
      digitalWrite(motor_l1,LOW);
      digitalWrite(motor_l2,LOW);
      digitalWrite(motor_r1,LOW);
      digitalWrite(motor_r2,LOW); 
      digitalWrite(ball_shooter,LOW);      
    }
    break;
  case 1:
    {
      //Serial.println("hand down");
      wrist_con--;
      wrist_con = constrain(wrist_con,25,107);
      grip_con = constrain(grip_con,97,145);
    }
    break;
  case 2:
    {
      //Serial.println("grip");
      grip_con--;
      wrist_con = constrain(wrist_con,25,107);
      grip_con = constrain(grip_con,97,145);
    }
    break;
  case 3:
    {
      //Serial.println("hand up");
      wrist_con++;
      wrist_con = constrain(wrist_con,25,107);
      grip_con = constrain(grip_con,97,145);
    }
    break;
  case 4:
    {
      //Serial.println("release");
     grip_con++;
     wrist_con = constrain(wrist_con,25,107);
     grip_con = constrain(grip_con,97,145); 
    }
    break;
  case 8:
    {
      blow_buzzer();
      base_speed+=5;
      base_speed = constrain(base_speed,0,255);
      //Serial.println(base_speed);
      delay(500);
    }
    break;
  case 6:
    {
      blow_buzzer();
      base_speed-=5;
      base_speed = constrain(base_speed,0,255);
      //Serial.println(base_speed);
      delay(500);
    }
    break;
  case 11:
    {
      analogWrite(motor_l1,base_speed);
      digitalWrite(motor_l2,0);
      analogWrite(motor_r1,base_speed);
      digitalWrite(motor_r2,0);
      //Serial.println("go forward");      
    }
    break;
  case 33:
    {
      digitalWrite(motor_l1,0);
      analogWrite(motor_l2,base_speed);
      digitalWrite(motor_r1,0);
      analogWrite(motor_r2,base_speed);
      //Serial.println("go backword");      
    }
    break;
  case 22:
    {      
      digitalWrite(motor_l1,0);
      analogWrite(motor_l2,base_speed);
      analogWrite(motor_r1,base_speed);
      digitalWrite(motor_r2,0);
      //Serial.println("go right");      
    }
    break;
  case 44:
    {
      analogWrite(motor_l1,base_speed);
      digitalWrite(motor_l2,0);
      digitalWrite(motor_r1,0);
      analogWrite(motor_r2,base_speed);
      //Serial.println("go left");
    }
    break;
  case 9:
    {
      digitalWrite(ball_shooter,HIGH);
      //Serial.println("shooting ball"); 
    }
    break;
  case 10:
    {
      blow_buzzer();
      re_start();
    }
    break;
  }
  delay(40);
}




void blow_buzzer()
{
  digitalWrite(bz_pin,HIGH);
  delay(50);
  digitalWrite(bz_pin,LOW);
}

