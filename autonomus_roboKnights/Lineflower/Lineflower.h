#ifndef Lineflower_h
#define Lineflower_h
#include "Arduino.h"
#include<GridM.h>
#include<Servo.h>
#include<EEPROM.h>
const byte column = 9;//
class Lineflower
{
public:
    int thres;//
	Lineflower(int kp_,int base_speed_,int sensor_thres_val_,GridM* g_);//
	void init_button(int button1,int button2,int button3,int button4);//
	void init_sonor1(int so_vcc,int so_gnd,int so_trig,int so_eco);//
	void init_sonor2(int so_vcc,int so_gnd,int so_trig,int so_eco);//
	void init_motor(int m_l_f,int m_l_b,int m_r_f,int m_r_b);//
	void init_sensor(int val[]);//
	void init_hand(int grip_,int wrist_);//
	void gripper(int ang);//
	void wrister(int ang);//
	void init_buzzer(int bz_pin);//
	void blow_buzzer();//
	int button_press_check();//
	void read_sensor();//
	void flow_line();//
	void get_error();//
	void calibrate();//
	void left_turn(int spd,unsigned int forward_time,unsigned int turn_time);//
	void right_turn(int spd,unsigned int forward_time,unsigned int turn_time);//
	void rotate_bot_to_given_position(int pos,int spd,unsigned int forward_time,unsigned int turn_time);//
	void left_wheel(int spd);//
	void right_wheel(int spd);//
	double get_distance_s1();//
	double get_distance_s2();//
	void init_turn(int turn_speed_,unsigned int forward_time_,unsigned int turn_time_);//
	void dry_run(unsigned int cross_section_time,unsigned int node_check_time,unsigned int node_or_block_base_detection_time);//
	void go(unsigned int cross_section_time);//
	void align_bot();//
	int box_range();//
	void take_permission_from_big_bro();//
	void wait(byte analog_pin,int thres_val);//
	void flow_lineb();//
	void out_restriction_zone(unsigned int cross_section_time);//
	void enter_restriction_zone(unsigned int cross_section_time);//
	void find_path_and_save();//
	void load_data();//
	void drop_box(unsigned int cross_time);//
	void avoid_pit();//
	int error1;//
	int error2;//
	int error;//
	int error_call_counter;//
	int current_direction;//
	GridM* game_field;//
	byte counter;//
	int block_base[2];//
	byte block_base_index;//
	String path_direction;//
	byte check_point;//
	int sen_max_data[10];//
	int sen_min_data[10];//
	int box_detect_min_value;//
	byte box_pit_min_dis;//
	byte down_angle;//
	byte up_angle;//
	byte grab_angle;//
	byte ir_proximity;
	byte release_angle;//
	boolean box_node;//
	boolean box_180;//
	byte turn_speed;//
	unsigned int forward_time;//
	unsigned int turn_time;//
	unsigned int last_flow_line_time;//
	byte base_speed;//
	unsigned int box_pit_time;//
	String path1;//
	String path2;//
	String path3;//
	String path4;//
	String path5;//
	int allign_value[5];//
	boolean dry_run_box_flag;//
	boolean avoid_pit_flag;//
	boolean wait_for_big_bro;//
private:
	boolean wait_for_permission;//
	Servo grip;//
	Servo wrist;//
	byte kp;//
	int max_speed;//
	int min_speed;//
	byte sensor_threshold_value;//
	byte bzr;//
	byte sonor_trigger_pin;//
	byte sonor_vcc;//
	byte sonor_gnd;//
	byte sonor_input_pin;// 

	byte sonor_l_tp;//
	byte sonor_l_ip;//
	byte sonor_l_vcc;//
	byte sonor_l_gnd;//

	byte btn1;//
	byte btn2;//
	byte btn3;//
	byte btn4;//

	byte led;//

	byte left_m_f;//
	byte left_m_b;//
	byte right_m_f;//
	byte right_m_b;//
	byte no_of_sensor;//
	int sensor_input_pin[10];//
};




#endif