#ifndef Lineflower_h
#define Lineflower_h
#include "Arduino.h"
class Lineflower
{
public:
	Lineflower(int kp_,int base_speed_,int sensor_thres_val_);//
	void init_button(int button1,int button2,int button3,int button4);//
	void init_sonor1(int so_vcc,int so_gnd,int so_trig,int so_eco);//
	void init_sonor2(int so_vcc,int so_gnd,int so_trig,int so_eco);//
	void init_motor(int m_l_f,int m_l_b,int m_r_f,int m_r_b);//
	void init_sensor(int val[]);//
	int button_press_check();//
	void read_sensor();//
	void flow_line();//
	void calibrate();//
	void left_wheel(int spd);//
	void right_wheel(int spd);//
	double get_distance_s1();//
	double get_distance_s2();//
	int error1;//
	int error2;//
	int error_call_counter;//
private:
	void get_error();//
	int kp;//
	int base_speed;//
	int max_speed;//
	int min_speed;//
	int error;//
	int sensor_threshold_value;//
	
	int sonor_trigger_pin;//
	int sonor_vcc;//
	int sonor_gnd;//
	int sonor_input_pin;// 

	int sonor_l_tp;//
	int sonor_l_ip;//
	int sonor_l_vcc;//
	int sonor_l_gnd;//

	int btn1;//
	int btn2;//
	int btn3;//
	int btn4;//

	int led;//

	int left_m_f;//
	int left_m_b;//
	int right_m_f;//
	int right_m_b;//
	int no_of_sensor;//
	int sensor_input_pin[10];//
	int sen_max_data[10];//
	int sen_min_data[10];//
};




#endif