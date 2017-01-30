#include<Servo.h>
Servo s1;
Servo sl;
Servo sr;
void setup()
{
  s1.attach(7);
  sl.attach(6);
  sr.attach(5);
  s1.write(30);
  up();
}
void loop()
{
  up();
  //grip();
  delay(3000);
  //release_();
  down();
  delay(3000);
}
void up()
{ 
  for(int i = 0;i<40;i++)
  {
    s1.write(60-i);
    delay(10);
  }
}
void down()
{ 
  for(int i = 0;i<40;i++)
  {
    s1.write(20+i);
    delay(10);
  }
}

void grip()
{
  for(int i = 0;i<50;i++)
  {
    sl.write(10+i);
    sr.write(170-i);
    delay(10);
  }
  delay(1000);  
}
void release_()
{
  for(int i = 0;i<50;i++)
  {
    sl.write(60-i);
    sr.write(120+i);
    delay(10);
  }
  delay(1000);
}
