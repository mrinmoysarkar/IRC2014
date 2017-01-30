#include<Servo.h>
Servo s1,s2;
int pang = 0;
void setup()
{
  s1.attach(5);
  s2.attach(7);
  s2.write(0);
  delay(2000);
  //s1.write(50);
  //while(true);
  Serial.begin(9600);
}
void loop()
{
  for(int i = 50;i<120;i++)
  {
    s1.write(i);
    delay(10);
  }
  delay(3000);
  for(int i = 120;i>50;i--)
  {
    s1.write(i);
    delay(10);
  }
  delay(3000);
  for(int i = 0;i<40;i++)
  {
    s2.write(i);
    delay(10);
  }
  while(true);
}
