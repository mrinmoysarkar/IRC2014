#include<Lineflower.h>
#include <GridM.h>
#define len_of_int 100
#define grid_size 16
const int r=4;
const int c=4;
int kp = 7;
int base_speed = 80;
int sensor_thresh_value = 80;
GridM g(r,c);
Lineflower robok(kp,base_speed,sensor_thresh_value,&g);
int sensor_pin[]={
  0,1,2,3,4,5,6,7,8,9};

int bzr = 22;

int cross_section_time = 300;
int node_check_time = 100;
int node_or_block_base_detection_time = 1000;
void setup()
{
  Serial.begin(9600);
  robok.init_turn(70,700,300);
  robok.init_buzzer(bzr);
  robok.init_button(4,5,2,3);
  robok.init_sensor(sensor_pin);
  robok.init_motor(10,11,9,8);
}
void loop()
{
  if(robok.button_press_check()==2)
  {
    robok.calibrate();
  }
  if(robok.button_press_check()==1)
  {
    robok.counter = 0;
    robok.block_base_index = 0;
    robok.dry_run(cross_section_time,node_check_time,node_or_block_base_detection_time);
    robok.right_wheel(0);
    robok.left_wheel(0);
  }
  else if(robok.button_press_check()==3)
  {
    robok.current_direction = 0;
    g.set_source(1);
    g.set_destination(robok.block_base[0]);
    robok.go(300);
    g.set_source(robok.block_base[0]);
    g.set_destination(39); 
    robok.go(300);
    g.set_source(39);
    g.set_destination(44);
    robok.go(300);
    //do something
    g.set_source(44);
    g.set_destination(robok.block_base[1]);
    robok.go(300);
    g.set_source(robok.block_base[1]);
    g.set_destination(45);
    robok.go(300);
    //do_something;
  }
  robok.right_wheel(0);
  robok.left_wheel(0);
  delay(300);
}


