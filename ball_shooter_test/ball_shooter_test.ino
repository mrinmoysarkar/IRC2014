#include<Servo.h>
Servo s1;
void setup()
{
  s1.attach(7);
}
void loop()
{
  for(int i = 0;i<=760;i++)
  {
    s1.write(i);
    delay(50);
  }
  delay(2000); 
}
