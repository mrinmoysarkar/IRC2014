#include<Lineflower.h>
#include <GridM.h>
#include<Servo.h>
#include<EEPROM.h>
#define len_of_int 100
#define grid_size 45
const byte r=5;
const byte c=9;
byte kp = 5;
byte base_speed = 110;
byte sensor_thresh_value = 40;
GridM arena(r,c);
Lineflower robo_Knights(kp,base_speed,sensor_thresh_value,&arena);
int sensor_pin[]={
  0,1,2,3,4,5,6,7,8,9};

byte bzr = 22;
byte bzr_gnd = 28;
byte turn_speed = 77;
int forward_delay = 350;
int turn_delay = 200;

int cross_section_time = 250;
int node_check_time = 40;
int node_or_block_base_detection_time = 350;
byte grip = 45;
byte weis = 46;
byte bb_ind = 0;
byte count = 0;
byte ldr_pin = 14;
int ldr_thres_value = 250;
boolean manue_change = true;
boolean  buzzer_flag = false;
void setup()
{
  pinMode(bzr_gnd,OUTPUT);
  pinMode(53,OUTPUT);
  pinMode(52,OUTPUT);
  digitalWrite(bzr_gnd,LOW);
  digitalWrite(53,HIGH);
  digitalWrite(52,HIGH);
  robo_Knights.up_angle = 130;
  robo_Knights.down_angle = 0;
  robo_Knights.grab_angle = 170;
  robo_Knights.release_angle = 40;
  robo_Knights.box_pit_min_dis = 170;
  robo_Knights.box_detect_min_value = 470;
  robo_Knights.ir_proximity = 15;
  robo_Knights.check_point = 0;
  robo_Knights.last_flow_line_time = 4700;
  robo_Knights.current_direction = 0;
  robo_Knights.box_pit_time = 200;
  robo_Knights.avoid_pit_flag = false;
  robo_Knights.wait_for_big_bro = true;

  Serial.begin(9600);
  robo_Knights.init_turn(turn_speed,forward_delay,turn_delay);
  robo_Knights.init_hand(grip,weis);
  robo_Knights.init_buzzer(bzr);
  robo_Knights.init_button(32,33,34,35);
  robo_Knights.init_sensor(sensor_pin);
  robo_Knights.init_motor(4,5,3,2);
  robo_Knights.wrister(robo_Knights.up_angle-25);
  robo_Knights.gripper(robo_Knights.release_angle+30);
  robo_Knights.load_data();
}
void loop()
{
  //robo_Knights.read_sensor();
 // /*
  if(robo_Knights.button_press_check()==1)
  {
    if(true)//manue_change)
    {
      robo_Knights.avoid_pit_flag = false;
      robo_Knights.calibrate();
      robo_Knights.thres = 57;
    }
    else
    {
      //robo_Knights.load_data();
    }
  }
  if(robo_Knights.button_press_check()==2)
  {
    // robo_Knights.out_restriction_zone(300);
    
    if(manue_change)
    {
      EEPROM.write(237,0);
      robo_Knights.avoid_pit_flag = false;
      robo_Knights.counter = count;
      robo_Knights.block_base_index = bb_ind;
      robo_Knights.dry_run(cross_section_time,node_check_time,node_or_block_base_detection_time);
      robo_Knights.right_wheel(0);
      robo_Knights.left_wheel(0);
      robo_Knights.find_path_and_save();
      robo_Knights.current_direction = 0;
      Serial.println("1st box");
      Serial.println(robo_Knights.block_base[0]);
      Serial.println("2nd box");
      Serial.println(robo_Knights.block_base[1]);
      Serial.println();
      arena.print_board();      
    }
    else
    {
      robo_Knights.check_point++;
      robo_Knights.check_point=constrain(robo_Knights.check_point,0,5);
    }
  }
  if(robo_Knights.button_press_check()==3)
  {
    if(manue_change)
    {
      if(robo_Knights.check_point == 0)
      {
        robo_Knights.wrister(robo_Knights.up_angle-25);
        robo_Knights.gripper(robo_Knights.release_angle+30);
        robo_Knights.current_direction = 0;
        robo_Knights.go(300); 
        robo_Knights.go(300);
        EEPROM.write(100,robo_Knights.check_point);
        delay(200);
      }
      if(robo_Knights.check_point == 2)
      {
        robo_Knights.up_angle = 120;
        robo_Knights.go(300);
        robo_Knights.rotate_bot_to_given_position(90,robo_Knights.turn_speed,robo_Knights.forward_time,robo_Knights.turn_time);      
        robo_Knights.enter_restriction_zone(300);
        robo_Knights.wait(ldr_pin,ldr_thres_value);
        robo_Knights.out_restriction_zone(300); 
        EEPROM.write(100,robo_Knights.check_point);
        delay(500);
        robo_Knights.wrister(robo_Knights.up_angle-25);
        robo_Knights.gripper(robo_Knights.release_angle+30);
      }
      if(robo_Knights.check_point == 3)
      {
        robo_Knights.up_angle = 120;
        robo_Knights.go(300);
        robo_Knights.go(300);
        EEPROM.write(100,robo_Knights.check_point);
        robo_Knights.drop_box(300);
      }
      else if(robo_Knights.check_point == 5)
      {
        robo_Knights.up_angle = 120;
        unsigned long tim = millis();
        while((millis()-tim) < robo_Knights.last_flow_line_time)
        {
          robo_Knights.flow_line();
        }
        robo_Knights.right_wheel(0);
        robo_Knights.left_wheel(0);  
      }
    }
    else
    {
      robo_Knights.check_point--;
      robo_Knights.check_point=constrain(robo_Knights.check_point,0,5);
    }
  }
  if(robo_Knights.button_press_check()==4)
  {
    robo_Knights.thres-=5;
    robo_Knights.check_point++;
    if(robo_Knights.check_point == 6)
    {
      robo_Knights.check_point = 0;
    }
    delay(500);
    for(int i = 0;i<=robo_Knights.check_point;i++)
    {
        robo_Knights.blow_buzzer();
        delay(500);
    }
    Serial.println(robo_Knights.check_point);
    //manue_change = !manue_change;
    robo_Knights.right_wheel(0);
    robo_Knights.left_wheel(0);
    //robo_Knights.check_point = 5;
    //robo_Knights.avoid_pit_flag = true;
    //robo_Knights.box_node = true;
    //robo_Knights.box_180 = true;
    //robo_Knights.left_turn(turn_speed,forward_delay,turn_delay);
    //robo_Knights.left_wheel(-80);
    //robo_Knights.right_wheel(80);
    //delay(1700);
    robo_Knights.right_wheel(0);
    robo_Knights.left_wheel(0);
    //robo_Knights.drop_box(300);
  }
  robo_Knights.right_wheel(0);
  robo_Knights.left_wheel(0);
  delay(100);//*/
}
/*
void speed_change()
{
  robo_Knights.right_wheel(0);
  robo_Knights.left_wheel(0);
  robo_Knights.forward_time = 370;
  robo_Knights.turn_time = 250;
  robo_Knights.base_speed = 120;
  robo_Knights.turn_speed = 120;
}

void clear_eeprom()
{
  for(int i = 0;i<=100;i++)
  {
    EEPROM.write(i,0);
    delay(50);
  }
}*/
/*
void all_path_generator()
{
  arena.set_node(robo_Knights.block_base[1]);
  arena.set_node(40);
  arena.set_source(1);
  arena.set_destination(robo_Knights.block_base[0]);
  robo_Knights.path1=arena.get_path();
  robo_Knights.allign_value[0] = arena.que[0]-arena.que[1]-1+10;
   
  arena.remove_node(40);
  arena.set_source(robo_Knights.block_base[0]);
  arena.set_destination(40); 
  robo_Knights.path2=arena.get_path();
  robo_Knights.allign_value[1] = arena.que[0]-arena.que[1]-1+10;
  
  
  arena.set_node(robo_Knights.block_base[0]);
  arena.set_node(robo_Knights.block_base[1]);
  arena.set_source(40);
  arena.set_destination(44);
  robo_Knights.path3=arena.get_path();
  robo_Knights.allign_value[2] = arena.que[0]-arena.que[1]-1+10;
  
  
  arena.remove_node(robo_Knights.block_base[1]);
  arena.set_source(44);
  arena.set_destination(robo_Knights.block_base[1]);
  robo_Knights.path4=arena.get_path();
  robo_Knights.allign_value[3] = arena.que[0]-arena.que[1]-1+10;
  
  arena.set_source(robo_Knights.block_base[1]);
  arena.set_destination(45);
  robo_Knights.path5=arena.get_path();
  robo_Knights.allign_value[4] = arena.que[0]-arena.que[1]-1+10;
  
  EEPROM.write(110,robo_Knights.allign_value[0]);
  delay(10);
  EEPROM.write(111,robo_Knights.allign_value[1]);
  delay(10);
  EEPROM.write(112,robo_Knights.allign_value[2]);
  delay(10);
  EEPROM.write(113,robo_Knights.allign_value[3]);
  delay(10);
  EEPROM.write(114,robo_Knights.allign_value[4]);
  byte len=robo_Knights.path1.length();
  EEPROM.write(120,len);
  delay(10);
  int mem = 120+len;
  for(int i = 0;i<len;i++)
  {
    EEPROM.write(121+i,robo_Knights.path1.charAt(i));
    delay(10);
  }

  len=robo_Knights.path2.length();
  mem=mem+3;
  EEPROM.write(mem,len);
  delay(10);
  mem=mem+1;
  for(int i = 0;i<len;i++)
  {
    EEPROM.write(mem+i,robo_Knights.path2.charAt(i));
    delay(10);
  }
  mem+=len;

  len=robo_Knights.path3.length();
  mem=mem+3;
  EEPROM.write(mem,len);
  mem=mem+1;
  delay(10);
  for(int i = 0;i<len;i++)
  {
    EEPROM.write(mem+i,robo_Knights.path3.charAt(i));
    delay(10);
  }
  mem+=len;

  len=robo_Knights.path4.length();
  mem=mem+3;
  EEPROM.write(mem,len);
  delay(10);
  mem=mem+1;
  for(int i = 0;i<len;i++)
  {
    EEPROM.write(mem+i,robo_Knights.path4.charAt(i));
    delay(10);
  }
  mem+=len;

  len=robo_Knights.path5.length();
  mem=mem+3;
  EEPROM.write(mem,len);
  delay(10);
  mem=mem+1;
  for(int i = 0;i<len;i++)
  {
    EEPROM.write(mem+i,robo_Knights.path5.charAt(i));
    delay(10);
  }
  Serial.println("original path");
  Serial.println(robo_Knights.path1);
  Serial.println(robo_Knights.path2);
  Serial.println(robo_Knights.path3);
  Serial.println(robo_Knights.path4);
  Serial.println(robo_Knights.path5);
  Serial.println("original path");
  robo_Knights.blow_buzzer();
}
*/
