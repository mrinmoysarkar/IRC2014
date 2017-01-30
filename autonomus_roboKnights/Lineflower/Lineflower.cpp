#include "Arduino.h"
#include "Lineflower.h"

Lineflower::Lineflower(int kp_,int base_speed_,int sensor_thres_val_,GridM* g_)
{
	thres = 57;
	avoid_pit_flag = false;
	dry_run_box_flag = false;
	wait_for_permission = true;
	wait_for_big_bro = true;
	game_field = g_;
	kp = kp_;
	path1="";
	path2="";
	path3="";
	path4="";
	path5="";
	ir_proximity = 10;
	check_point = 0;
	path_direction = "";
	block_base[0] = 0;
	block_base[1] = 0;
	block_base_index = 0;
	base_speed = base_speed_;
	sensor_threshold_value = sensor_thres_val_;
	min_speed = -130;
	max_speed = 130;
	error = 0;
	error1 = 0;
	error2 = 0;
	box_pit_min_dis = 0;
	box_detect_min_value = 0;
	current_direction = 0;
	error_call_counter = 0;
	last_flow_line_time = 0;
	no_of_sensor = 10;
	led = 13;
	pinMode(led,OUTPUT);
	for(int i = 0;i<no_of_sensor;i++)
	{
		sen_max_data[i] = 0;
		sen_min_data[i] = 1023;
	}
}

void Lineflower::init_button(int button1,int button2,int button3,int button4)
{
	btn1 = button1;
	btn2 = button2;
	btn3 = button3;
	btn4 = button4;
	pinMode(btn1,INPUT);
	pinMode(btn2,INPUT);
	pinMode(btn3,INPUT);
	pinMode(btn4,INPUT);
}
void Lineflower::init_sonor1(int so_vcc,int so_gnd,int so_trig,int so_eco)
{
	sonor_vcc = so_vcc;
	sonor_gnd = so_gnd;
	sonor_trigger_pin = so_trig;
	sonor_input_pin = so_eco;
    pinMode(sonor_vcc,OUTPUT);
	digitalWrite(sonor_vcc,HIGH);
	pinMode(sonor_gnd,OUTPUT);
	digitalWrite(sonor_gnd,LOW);
	pinMode(sonor_trigger_pin,OUTPUT);
	pinMode(sonor_input_pin,INPUT);
}
void Lineflower::init_sonor2(int so_vcc,int so_gnd,int so_trig,int so_eco)
{
	sonor_l_vcc = so_vcc;
	sonor_l_gnd = so_gnd;
	sonor_l_tp = so_trig;
	sonor_l_ip = so_eco;
	pinMode(sonor_l_tp,OUTPUT);
	pinMode(sonor_l_ip,INPUT);
	pinMode(sonor_l_vcc,OUTPUT);
	pinMode(sonor_l_gnd,OUTPUT);
	digitalWrite(sonor_l_vcc,HIGH);
	digitalWrite(sonor_l_gnd,LOW);
}
void Lineflower::init_motor(int m_l_f,int m_l_b,int m_r_f,int m_r_b)
{
	left_m_f = m_l_f;
	left_m_b = m_l_b;
	right_m_f = m_r_f;
	right_m_b = m_r_b;
	pinMode(left_m_f,OUTPUT);
	pinMode(left_m_b,OUTPUT);
	pinMode(right_m_f,OUTPUT);
	pinMode(right_m_b,OUTPUT);	
}
void Lineflower::init_sensor(int val[])
{
	for(int i = 0;i < no_of_sensor;i++)
	{
		sensor_input_pin[i] = val[i];
	}
}
int Lineflower::button_press_check()
{
  if(digitalRead(btn1) == LOW)
  {
    blow_buzzer();
    return 1;
  }
  else if(digitalRead(btn2) == LOW)
  {
    blow_buzzer();
    return 2;
  }
  else if(digitalRead(btn3) == LOW)
  {
    blow_buzzer();
    return 3;
  }
  else if(digitalRead(btn4) == LOW)
  {
    blow_buzzer();
    return 4;
  }
  return 0;
}
void Lineflower::read_sensor()
{
  for(int i = 0;i<no_of_sensor;i++)
  {
    Serial.println(analogRead(sensor_input_pin[i]));
  }
  Serial.println("##########################");
  delay(500);
}
void Lineflower::flow_line()
{
  get_error();
  int turn = kp*error;
  left_wheel(base_speed - turn);
  right_wheel(base_speed + turn);
}
void Lineflower::flow_lineb()
{
  get_error();
  int turn = (kp)*error;
  left_wheel(-base_speed - turn);
  right_wheel(-base_speed + turn);
}
void Lineflower::calibrate()
{
  wrister(up_angle-20);
  gripper(release_angle+70);
  unsigned long time = millis();
  int spd = 70;
  for(int i=0;i<2;i++)
  {
    left_wheel(spd);
    right_wheel(-spd);
    time = millis();
    while((millis() - time) < 1200)
    {
      for(int i = 0;i < 10;i++)
      {
        int data = analogRead(sensor_input_pin[i]);
        if(data > sen_max_data[i])
        {
          sen_max_data[i] = data;
        }
        if(data < sen_min_data[i])
        {
          sen_min_data[i] = data;
        }
      }
    }
    time = millis();
    left_wheel(-spd);
    right_wheel(spd);
    while((millis() - time) < 1250)
    {
      for(int i = 0;i < 10;i++)
      {
        int data = analogRead(sensor_input_pin[i]);
        if(data > sen_max_data[i])
        {
          sen_max_data[i] = data;
        }
        if(data < sen_min_data[i])
        {
          sen_min_data[i] = data;
        }
      }
    }
    left_wheel(0);
    right_wheel(0);
  }
  Serial.println("max sensor data");
  for(int i = 0,j = 0;i<10;i++,j+=2)
  {
	//EEPROM.write(j,byte(sen_max_data[i]));
	delay(10);
	//EEPROM.write(j+1,byte(sen_max_data[i]>>8));
	delay(10);
    Serial.print(sen_max_data[i]);
    Serial.print("    ");
  }
  Serial.println("\nmin sensor data");
  for(int i= 0;i<10;i++)
  {
	//EEPROM.write(i+20,byte(sen_min_data[i]));
	delay(10);
    Serial.print(sen_min_data[i]);
    Serial.print("    ");
  }
}
void Lineflower::left_wheel(int spd)
{
  spd = constrain(spd,min_speed,max_speed);
  if(spd >= 0)
  {
    analogWrite(left_m_f,spd);
    analogWrite(left_m_b,0);
  }
  else
  {
    analogWrite(left_m_b,-spd);
    analogWrite(left_m_f,0);
  }
}
void Lineflower::right_wheel(int spd)
{
  spd = constrain(spd,min_speed,max_speed);
  if(spd >= 0)
  {
    analogWrite(right_m_f,spd);
    analogWrite(right_m_b,0);
  }
  else
  {
    analogWrite(right_m_b,-spd);
    analogWrite(right_m_f,0);
  }
}
double Lineflower::get_distance_s1()
{
  digitalWrite(sonor_trigger_pin,HIGH);
  delayMicroseconds(10);
  digitalWrite(sonor_trigger_pin,LOW);
  int i = pulseIn(sonor_input_pin,HIGH);
  double distance = 0.017 * i;//distance in cm
  return distance;
}

double Lineflower::get_distance_s2()
{
  digitalWrite(sonor_l_tp,HIGH);
  delayMicroseconds(10);
  digitalWrite(sonor_l_tp,LOW);
  int i = pulseIn(sonor_l_ip,HIGH);
  double distance = 0.017 * i;//distance in cm
  return distance;
}

void Lineflower::get_error()
{
  error1 = 0;
  error2 = 0;
  error = 0;
  int data = analogRead(sensor_input_pin[0]);
  error_call_counter++;

  data = analogRead(sensor_input_pin[0]);
  data = constrain(data,sen_min_data[0],sen_max_data[0]);
  data = map(data,sen_min_data[0],sen_max_data[0],0,256);
  error1 += data < sensor_threshold_value?-7:0;


  data = analogRead(sensor_input_pin[1]);
  data = constrain(data,sen_min_data[1],sen_max_data[1]);
  data = map(data,sen_min_data[1],sen_max_data[1],0,256);
  error1 += data < sensor_threshold_value?-5:0;

  data = analogRead(sensor_input_pin[2]);
  data = constrain(data,sen_min_data[2],sen_max_data[2]);
  data = map(data,sen_min_data[2],sen_max_data[2],0,256);
  error1 += data < sensor_threshold_value?-3:0;

  data = analogRead(sensor_input_pin[3]);
  data = constrain(data,sen_min_data[3],sen_max_data[3]);
  data = map(data,sen_min_data[3],sen_max_data[3],0,256);
  error1 += data < sensor_threshold_value?-2:0;

  data = analogRead(sensor_input_pin[4]);
  data = constrain(data,sen_min_data[4],sen_max_data[4]);
  data = map(data,sen_min_data[4],sen_max_data[4],0,256);
  error1 += data < sensor_threshold_value?-1:0;

  data = analogRead(sensor_input_pin[5]);
  data = constrain(data,sen_min_data[5],sen_max_data[5]);
  data = map(data,sen_min_data[5],sen_max_data[5],0,256);
  error2 += data < sensor_threshold_value?1:0;

  data = analogRead(sensor_input_pin[6]);
  data = constrain(data,sen_min_data[6],sen_max_data[6]);
  data = map(data,sen_min_data[6],sen_max_data[6],0,256);
  error2 += data < sensor_threshold_value?2:0;

  data = analogRead(sensor_input_pin[7]);
  data = constrain(data,sen_min_data[7],sen_max_data[7]);
  data = map(data,sen_min_data[7],sen_max_data[7],0,256);
  error2 += data < sensor_threshold_value?3:0;

  data = analogRead(sensor_input_pin[8]);
  data = constrain(data,sen_min_data[8],sen_max_data[8]);
  data = map(data,sen_min_data[8],sen_max_data[8],0,256);
  error2 += data < sensor_threshold_value?5:0;

  data = analogRead(sensor_input_pin[9]);
  data = constrain(data,sen_min_data[9],sen_max_data[9]);
  data = map(data,sen_min_data[9],sen_max_data[9],0,256);
  error2 += data < sensor_threshold_value?7:0;

  error = error1 + error2;
}

void Lineflower::left_turn(int spd,unsigned int forward_time,unsigned int turn_time)
{
  unsigned long time = 0;
  current_direction += 270;
  current_direction %= 360;
  left_wheel(spd);
  right_wheel(spd);
  if(box_node)
  {
	time = millis();
	if(dry_run_box_flag)
	{
		dry_run_box_flag = false;
		while((millis()-time)<.5*forward_time)
		{
			flow_line();
		}
	}
	else
	{
		if(!box_180)
		{
			while((millis()-time)<1.5*forward_time)
			{
				flow_line();
			}
		}
	}	
		/*
	time = millis();
	while((millis()-time)<1000 && error1<=-12 && error2>=12)
	{
		flow_line();
	}
	left_wheel(spd);
	right_wheel(spd);
	delay(200);*/
	if(box_180)
	{
		//left_wheel(-spd);
		//right_wheel(spd);
		current_direction += 270;
		current_direction %= 360;
		delay(forward_time);
		left_wheel(-spd);
		right_wheel(spd);
		delay(forward_time);
		get_error();
		time = millis();
		unsigned long timeee = millis();
		while((error1!=0 || error2!=0)&&(millis()-timeee)<900)
		{
			get_error();
		}
		goto lebel;
		
		/*
		if(check_point == 4)
		{
			delay(6.3*forward_time);
		}
		else
		{
			delay(5.5*forward_time);
		}*/
	}
	else
	{
		left_wheel(-spd);
		right_wheel(spd);
		delay(600);
		left_wheel(spd);
		right_wheel(spd);
		get_error();
		unsigned long timeee = millis();
		while((error1!=0 && error2!=0)&&(millis()-timeee)<500)
		{
			get_error();
		}
		delay(100);
		goto lebel;
	
	/*
		left_wheel(-spd);
		right_wheel(spd);
		if(check_point == 4)
		{
			delay(2.7*forward_time);
		}
		else
		{
			delay(2.5*forward_time);
		}*/
	}
		/*
	time = millis();
	unsigned long tm = millis();
	while((millis()-time)<300)
	{
		flow_line();
		if((millis()-tm) > 150 )
		{
			get_error();
			if(error1 == 0 && error2 == 0)
			{
				left_wheel(-spd);
				right_wheel(spd);
				while(error1 == 0 && error2 == 0)
				{
					get_error();
				}
			}
			tm +=400; 
		}
	}
	left_wheel(0);
	right_wheel(0);
	return;*/
  }
  time=millis();
  while((millis()-time)<forward_time)
  {
	flow_line();
  }
lebel:
  left_wheel(-spd);
  right_wheel(spd);
  delay(turn_time);
  time = millis();
  unsigned long tm = 1050;
  if(box_node)
  {
	tm = 600;
	box_node = false;
  }
  if(box_180)
  {
	tm = 1400;
  }
  while(true)
  {
    get_error();
    boolean flg =error==15 || error==10 || error == 6;
    if(flg || (millis()-time)>tm)
    {
      left_wheel(0);
      right_wheel(0);
      break;
    }
  }
  tm = 100;
  time = millis();
  if(box_180)
  {
	left_wheel(-spd);
	right_wheel(spd);
	delay(30);
    tm = 50;
	box_180=false;
  }
  while((millis()-time)<tm)
  {
	flow_line();
  }
  left_wheel(0);
  right_wheel(0);
}
void Lineflower::right_turn(int spd,unsigned int forward_time,unsigned int turn_time)
{
  unsigned long time = 0;
  current_direction += 90;
  current_direction %= 360;
  left_wheel(spd);
  right_wheel(spd);
  if(box_node)
  {
	time = millis();
	if(dry_run_box_flag)
	{
		dry_run_box_flag = false;
		while(millis()-time<.5*forward_time)
		{
			flow_line();
		}
	}
	else
	{
		while(millis()-time<1.5*forward_time)
		{
			flow_line();
		}
	}
	/*
	time = millis();
	while((millis()-time)<1000 && error1<=-12 && error2>=12)
	{
		flow_line();
	}
	left_wheel(spd);
	right_wheel(spd);
	delay(200);*/
	left_wheel(spd);
	right_wheel(-spd);
	delay(600);
	left_wheel(spd);
	right_wheel(spd);
	get_error();
	unsigned long timeee = millis();
	while((error1!=0 && error2!=0) && (millis()-timeee)<450)
	{
		get_error();
	}
	delay(100);
	goto lebelr;
	/*
	if(check_point == 4)
	{
		delay(2.7*forward_time);
	}
	else
	{
		delay(2.5*forward_time);
	}
	time = millis();
	unsigned long tm = millis();
	while((millis()-time)<300)
	{
		flow_line();
		if((millis()-tm) > 150 )
		{
			get_error();
			if(error1 == 0 && error2 == 0)
			{
				left_wheel(spd);
				right_wheel(-spd);
				while(error1 == 0 || error2 == 0)
				{
					get_error();
				}
			}
			tm +=400; 
		}
	}
	left_wheel(0);
	right_wheel(0);
	return;*/
  }
  time=millis();
  while(millis()-time<forward_time)
  {
	flow_line();
  }
lebelr:
  left_wheel(spd);
  right_wheel(-spd);
  delay(turn_time);
  time = millis();
  unsigned long tm = 1050;
  if(box_node)
  {
	tm = 550;
	box_node = false;
  }
  while(true)
  {
    get_error();
    boolean flg =error==-15 || error==-10 || error==-6;
    if(flg || (millis()-time)>tm)
    {
      left_wheel(0);
      right_wheel(0);
      break;
    }
  }
  time = millis();
  while((millis()-time)<100)
  {
	flow_line();
  }
  left_wheel(0);
  right_wheel(0);
}

void Lineflower::rotate_bot_to_given_position(int pos,int spd,unsigned int forward_time,unsigned int turn_time)
{
  if(pos == current_direction)
  {
	left_wheel(spd);
	right_wheel(spd);
	delay(forward_time);
	if(box_node)
	{
		delay(2.5*forward_time);
	}
	left_wheel(0);
	right_wheel(0);	
    return;
  }
  else if(abs(pos - current_direction)==180)
  {
	if(box_node)
	{
		box_180 = true;
		left_turn(spd,forward_time,turn_time);
		return;
	}
    left_turn(spd,forward_time,turn_time);
	left_turn(spd,0,turn_time*2.5);
    return;
  }
  else
  {
    switch(pos)
    {
      case 0:
      {
        switch(current_direction)
        {
          case 90:
          {
            left_turn(spd,forward_time,turn_time);
          }
          break;
          case 270:
          {
            right_turn(spd,forward_time,turn_time);
          }
          break;
        }
      }
      break;
      case 90:
       {
        switch(current_direction)
        {
          case 180:
          {
            left_turn(spd,forward_time,turn_time);
          }
          break;
          case 0:
          {
            right_turn(spd,forward_time,turn_time);
          }
          break;
        }
      }
      break;
      case 180:
      {
        switch(current_direction)
        {
          case 270:
          {
            left_turn(spd,forward_time,turn_time);
          }
          break;
          case 90:
          {
            right_turn(spd,forward_time,turn_time);
          }
          break;
        }
      }
      break;
      case 270:
      {
        switch(current_direction)
        {
          case 180:
          {
            right_turn(spd,forward_time,turn_time);
          }
          break;
          case 0:
          {
            left_turn(spd,forward_time,turn_time);
          }
          break;
        }
      }
      break;
    }
  }
}
void Lineflower::init_buzzer(int bz_pin)
{
	bzr = bz_pin;
	pinMode(bz_pin,OUTPUT);
}

void Lineflower::blow_buzzer()
{
  digitalWrite(bzr,HIGH);
  delay(50);
  digitalWrite(bzr,LOW);
}

void Lineflower::init_turn(int turn_speed_,unsigned int forward_time_,unsigned int turn_time_)
{
	turn_speed = turn_speed_;
	forward_time = forward_time_;
	turn_time = turn_time_;
}

void Lineflower::dry_run(unsigned int cross_section_time,unsigned int node_check_time,unsigned int node_or_block_base_detection_time)
{
	//boolean nodeorboxflag = false;
	block_base[0]=0;
	block_base[1]=0;
	for(int i = 0;i < game_field->r*game_field->c;i++)
	{
		game_field->board[i] = 0;
	}
	check_point = 0;
    //unsigned long time1 = millis();
    int row = 0;
    int col = 0;
    int node_point = 0;

	int counter = 0;
	while(true)
	{
		flow_line();
		int countre = 0;
		if(error1 <= -12 && error2 >= 12)
		{
			unsigned long time=millis();
			while((millis()-time)<node_or_block_base_detection_time)
			{
				flow_line();
				if(error1 <= -12 && error2 >= 12)
				{
					if(error1 != -18 || error2 != 18)
					{
						countre++;
					}
				}
			}
			left_wheel(0);
			right_wheel(0);
			counter++;
			row = (counter-1)/game_field->c+1;
			col = (counter-1)%game_field->c+1;
			if(row%2 == 0)
			{
				node_point = row*game_field->c-col+1;
			}
			else
			{
				node_point = counter;
			}

			if(countre>1 && countre <thres)
			{
				if(!(counter == 9 || counter == 10 ||counter == 27 ||counter == 28 || counter == 18 || counter == 19 || counter == 36 || counter == 37))
				{
					flow_line();
				}
				game_field->set_node(node_point);
				blow_buzzer();
			}
			else if(countre>=thres)
			{
				blow_buzzer();
				delay(50);
				blow_buzzer();
				if(block_base_index < 2)
				{
					if(block_base[block_base_index] == 0)
					{
						block_base[block_base_index] = node_point;
						block_base_index++;
					}
				}
				if(!(counter == 9 || counter == 10 ||counter == 27 ||counter == 28 || counter == 18 || counter == 19 || counter == 36 || counter == 37))
				{
					time=millis();
					while((millis()-time)<node_or_block_base_detection_time)
					{
						flow_line();
					}
				}
			}
		
			if(counter == 9 || counter == 10 ||counter == 27 ||counter == 28)
			{
				if(countre>50)
				{
					box_node = true;
					dry_run_box_flag = true;
					right_turn(turn_speed,forward_time-100,turn_time);
				}
				else
				{
					right_turn(turn_speed,0,turn_time);
				}
			
			}
			if(counter == 18 || counter == 19 || counter == 36 || counter == 37)
			{
				if(countre>50)
				{
					dry_run_box_flag = true;
					box_node = true;
					left_turn(turn_speed,forward_time-100,turn_time);
				}
				else
				{
					left_turn(turn_speed,0,turn_time);
				}
			}
		}
		if(counter == 45)
		{
			left_wheel(0);
			right_wheel(0);
			break;
		}
	}


	left_wheel(0);
	right_wheel(0);
	int bx1 = block_base[0]%game_field->c;
	bx1	= bx1 == 0?game_field->c:bx1;
	
	int bx2 = block_base[1]%game_field->c;
	bx2	= bx2 == 0?game_field->c:bx2;
	if(bx1>bx2)
	{
		bx1 = block_base[0];
		block_base[0] = block_base[1];
		block_base[1] = bx1;
		
	}	
	EEPROM.write(31,block_base[0]);
	delay(10);
	EEPROM.write(32,block_base[1]);
	delay(10);
	EEPROM.write(100,check_point);
	delay(10);
	game_field->write_board_to_eeprom();









/*
	boolean nodeorboxflag = false;
	block_base[0]=0;
	block_base[1]=0;
	for(int i = 0;i < game_field->r*game_field->c;i++)
	{
		game_field->board[i] = 0;
	}
	check_point = 0;
    unsigned long time1 = millis();
    int row = 0;
    int col = 0;
    int node_point = 0;
    while(true)
    {
	  nodeorboxflag = false;
      flow_line();
      if((millis()-time1)>cross_section_time&&(error1 <= -12 && error2 >= 12))
      {
        counter++;
        row = (counter-1)/game_field->c+1;
        col = (counter-1)%game_field->c+1;
        if(row%2 == 0)
        {
          node_point = row*game_field->c-col+1;
        }
        else
        {
          node_point = counter;
        }
        delay(node_check_time);
        left_wheel(0);
        right_wheel(0);
        get_error();
        if(error1 != -18 || error2 != 18)
        {
          if((millis()-time1) > node_or_block_base_detection_time)
          {
            game_field->set_node(node_point);
			nodeorboxflag = true;
          }
          else
          {
			if(row%2 == 0)
			{
				game_field->remove_node(node_point+1);
			}
			else
			{
				game_field->remove_node(node_point-1);
			}
            counter--;
			if(block_base_index < 2)
			{
				if(block_base[block_base_index] == 0)
				{
					block_base[block_base_index] = row%2 == 0?(node_point+1):(node_point - 1);
					block_base_index++;
				}
			}
          }
		  flow_line();
          blow_buzzer();
        }
        if(counter == 9 || counter == 10 ||counter == 27 ||counter == 28)
        {
			if(nodeorboxflag)
		    {
				nodeorboxflag= false;
				time1 = millis();
				while(millis()-time1<400)
				{
					flow_line();
				}
				left_wheel(0);
				right_wheel(0);
				get_error();
				if((error1 != -18 || error2 != 18) &&(error1 <= -12 && error2 >= 12))
				{
					if(block_base_index < 2)
					{
						if(block_base[block_base_index] == 0)
						{
							game_field->remove_node(node_point);
							block_base[block_base_index] = node_point;
							block_base_index++;
							blow_buzzer();
							left_wheel(turn_speed);
							right_wheel(turn_speed);
							delay(150);
							left_wheel(turn_speed);
							right_wheel(-turn_speed);
							delay(1.5*forward_time);
							time1 = millis();
							while(millis()-time1<400)
							{
								flow_line();
							}
						}
					}	
				}
				else
				{
					right_turn(turn_speed,0,turn_time);
				}
			}
		    else
		    {
				right_turn(turn_speed,forward_time,turn_time);
		    }
		    time1 = millis();
        }
        if(counter == 18 || counter == 19 || counter == 36 || counter == 37)
        {
			if(nodeorboxflag)
		    {
				nodeorboxflag= false;
				time1 = millis();
				while(millis()-time1<400)
				{
					flow_line();
				}
				left_wheel(0);
				right_wheel(0);
				get_error();
				if((error1 != -18 || error2 != 18) &&(error1 <= -12 && error2 >= 12))
				{
					if(block_base_index < 2)
					{
						if(block_base[block_base_index] == 0)
						{
							game_field->remove_node(node_point);
							block_base[block_base_index] = node_point;
							block_base_index++;
							blow_buzzer();
							left_wheel(turn_speed);
							right_wheel(turn_speed);
							delay(150);
							left_wheel(-turn_speed);
							right_wheel(turn_speed);
							delay(1.5*forward_time);
							time1 = millis();
							while(millis()-time1<400)
							{
								flow_line();
							}
						}
					}	
				}
				else
				{
					left_turn(turn_speed,0,turn_time);
				}
			}
		    else
		    {
				left_turn(turn_speed,forward_time,turn_time);
		    }
			time1 = millis();
        }
        time1 = millis();
        if(counter == game_field->r*game_field->c)
        {
		  if(nodeorboxflag)
		  {
			if(row%2 == 0)
			{
				game_field->remove_node(node_point+1);
			}
			else
			{
				game_field->remove_node(node_point-1);
			}
			if(block_base_index < 2)
			{
				if(block_base[block_base_index] == 0)
				{
					block_base[block_base_index] = row%2 == 0?(node_point+1):(node_point - 1);
					block_base_index++;
				}
			}	
		  }
          break;
        }
	  }
    }
	left_wheel(0);
	right_wheel(0);
	int bx1 = block_base[0]%game_field->c;
	bx1	= bx1 == 0?game_field->c:bx1;
	
	int bx2 = block_base[1]%game_field->c;
	bx2	= bx2 == 0?game_field->c:bx2;
	if(bx1>bx2)
	{
		bx1 = block_base[0];
		block_base[0] = block_base[1];
		block_base[1] = bx1;
		
	}	
	EEPROM.write(31,block_base[0]);
	delay(10);
	EEPROM.write(32,block_base[1]);
	delay(10);
	EEPROM.write(100,check_point);
	delay(10);
	game_field->write_board_to_eeprom();
	*/
}

void Lineflower::go(unsigned int cross_section_time)
{	
	switch(check_point)
	{
		case 0:
		{
			path_direction = path1;
		}
		break;
		case 1:
		{
			path_direction = path2;
		}
		break;
		case 2:
		{
			path_direction = path3;
		}
		break;
		case 3:
		{
			 path_direction = path4;
		}
		break;
		case 4:
		{
			path_direction = path5;
		}
		break;
	}
    counter = -1;
    unsigned long time1 = millis();
	if(check_point == 0)
	{
		while(true)
		{
			wrister(up_angle-10);
			flow_line();
			if((millis()-time1)>cross_section_time&&(error1 <= -12 && error2 >= 12))
			{
				break;
			}
		}
	}
	align_bot();
	time1 = millis();
	boolean box_up_flag = true;
    while(true)
    {
      flow_line();
	  if((check_point == 0 || check_point == 3 ) &&(((path_direction.length()-1) == counter)||(path_direction.length()==0)))
	  {
		if((box_range() > 100) && box_up_flag)
		{
			box_up_flag = false;
			unsigned long tim = millis();
			if((path_direction.length()!=0)&&(path_direction.charAt(path_direction.length()-1) != 'S'))
			{
				tim = millis();
				while((millis()-tim)<200)
				{
					flow_line();
				}
				tim = millis();
				while((millis()-tim)<500)
				{
					flow_lineb();
				}
				left_wheel(0);
				right_wheel(0);
				gripper(release_angle);
				delay(100);
				wrister(down_angle);
				delay(100);
				tim = millis();
				tim = millis();
				while((millis()-tim)<130)
				{
					flow_line();
				}
				tim = millis();
				while((millis()-tim)<370)
				{
					flow_line();
					if((box_range()<=box_detect_min_value))
					{
						left_wheel(0);
						right_wheel(0);
						delay(20);
						if((box_range()<=box_detect_min_value))
						{
							tim = 0;
							break;
						}
					}
				}
			}
			else
			{
				left_wheel(0);
				right_wheel(0);
				gripper(release_angle);
				delay(100);
				wrister(down_angle);
				delay(100);
				tim = millis();
				while((millis()-tim)<350)
				{
					flow_line();
				}
				tim = millis();
				while((millis()-tim)<370)
				{
					flow_line();
					if((box_range()<=box_detect_min_value))
					{
						left_wheel(0);
						right_wheel(0);
						delay(20);
						if((box_range()<=box_detect_min_value))
						{
							tim = 0;
							break;
						}
					}
				}
			}
			left_wheel(0);
			right_wheel(0);
			delay(100);
			gripper(grab_angle);
			delay(100);
			wrister(up_angle);
			if(check_point == 0)
			{
				delay(200);
				gripper(release_angle);
				delay(250);
				wrister(up_angle-30);
				delay(300);
				gripper(grab_angle-20);
				delay(100);
			}
			else if(check_point == 3)
			{
				base_speed += 5;
				turn_speed += 5;
				min_speed -=5;
				max_speed +=5;
				delay(300);
			}
			time1 = millis();
		}
		
	  }
	  if((check_point == 1 ) &&((path_direction.length()-1) == counter) && box_up_flag)
	  {
		box_up_flag = false;
		unsigned long tim = millis();
		int dl = 50;
		if(path_direction.charAt(path_direction.length()-2)=='S')
		{
			dl = 150;
		}
		while((millis()-tim)<dl)
		{
			flow_line();
		}
		//
		left_wheel(-base_speed);
		right_wheel(-base_speed);
		delay(400);
		left_wheel(0);
		right_wheel(0);
		//
		if(true)//wait_for_big_bro)
		{
			take_permission_from_big_bro();
		}
		time1 = millis();
	  }
      if((millis()-time1)>cross_section_time&&(error1 <= -12 && error2 >= 12))
      {
        counter++;
        if(counter == path_direction.length())
		{
			break;
		}
        if(path_direction.charAt(counter) == 'R')
        {
          right_turn(turn_speed,forward_time,turn_time);
        }
        else if(path_direction.charAt(counter) == 'L')
        {
          left_turn(turn_speed,forward_time,turn_time);
        }
        time1 = millis();
      }
    }
	check_point++;
	if(check_point == 1)
	{
		EEPROM.write(250,1);
		delay(100);
	}
	box_node = false;
	left_wheel(0);
	right_wheel(0);
	
}

void Lineflower::align_bot()
{ 
	if(check_point == 1 || check_point == 4)
	{
		box_node = true;
	}
	path_direction=path_direction.substring(1);
	int diff = 0;
	int tim = forward_time;
	if(check_point == 3)
	{
		forward_time = 70;
	}
	switch(check_point)
	{
		case 0:
		{
			diff = allign_value[0];
		}
		break;
		case 1:
		{
			diff = allign_value[1];
		}
		break;
		case 2:
		{
			diff = allign_value[2];
		}
		break;
		case 3:
		{
			 diff = allign_value[3];
		}
		break;
		case 4:
		{
			diff = allign_value[4];
		}
		break;
	}
	switch(diff)
	{
	 	case -1:
		{
			rotate_bot_to_given_position(0,turn_speed,forward_time,turn_time);
		}
		break;
		case 1:
		{
			rotate_bot_to_given_position(180,turn_speed,forward_time,turn_time);
		}
		break;
		case -column:
		{
			rotate_bot_to_given_position(90,turn_speed,forward_time,turn_time);
		}
		break;
		case column:
		{
			 rotate_bot_to_given_position(270,turn_speed,forward_time,turn_time);
		}
		break;
	}

	forward_time = tim;
}

void Lineflower::gripper(int ang)
{
	int val = grip.read();
	if(val<=ang)
	{
		for( ;val <= ang;val++)
		{
			grip.write(val);
			delay(10);
		}
	}
	else
	{
		for( ;val>=ang;val--)
		{
			grip.write(val);
			delay(10);
		}
	}
}
void Lineflower::wrister(int ang)
{
	int val = wrist.read();
	if(val<=ang)
	{
		for( ;val <= ang;val++)
		{
			wrist.write(val);
			delay(10);
		}
	}
	else
	{
		for( ;val >= ang;val--)
		{
			wrist.write(val);
			delay(10);
		}
	}
}

void Lineflower::init_hand(int grip_,int wrist_)
{
	grip.attach(grip_);
	wrist.attach(wrist_);
}

int Lineflower::box_range()
{
	return analogRead(ir_proximity);
}
void Lineflower::take_permission_from_big_bro()
{
	left_wheel(0);
	right_wheel(0);
	while(true)
	{
		unsigned long distance = 0;
		int i=0;
		unsigned long time = millis();
		while((millis()-time) < 1000)
		{
			int temp = box_range(); 
			distance += temp<20?box_pit_min_dis:temp;
			i++;
		}
		if(distance/i >= box_pit_min_dis)
		{
			break;
		}
		delay(700);
	}
	blow_buzzer();
	delay(2000);
	while(true)
	{
		unsigned long distance = 0;
		int i=0;
		unsigned long time = millis();
		while((millis()-time) < 2500)
		{
			int temp = box_range(); 
			distance += temp<60?box_pit_min_dis:temp;
			i++;
			delay(70);
		}
		delay(700);
		if(distance/i <= box_pit_min_dis)
		{
			break;
		}
		else
		{
			blow_buzzer();
			delay(500);
			blow_buzzer();
			delay(2000);
			take_permission_from_big_bro();
		}
		
	}
}

void Lineflower::wait(byte analog_pin,int thres_val)
{
	while(true)
	{
		unsigned long time = millis();
		long analog_value = 0;
		int i = 0;
		while((millis()-time)<3000)
		{
			i++;
			analog_value += analogRead(analog_pin);
			delay(50);
		}
		if(analog_value/i < thres_val)
		{
			blow_buzzer();
			time = millis();
			analog_value = 0;
			i = 0;
			while((millis()-time)<5000)
			{
				i++;
				analog_value += analogRead(analog_pin);
				delay(50);
			}
			if(analog_value/i < thres_val)
			{
				return;
			}
		}
	}
}

void Lineflower::enter_restriction_zone(unsigned int cross_section_time)
{
  unsigned long time1 = millis();
  int cont = 0;
  while(true)
  {
    flow_line();
    if((millis()-time1) > cross_section_time&&(error1 <= -12 && error2 >= 12))
    {
      cont++;
      if(cont == 2)
      {
		left_wheel(0);
		right_wheel(0);
        break;
      }
      time1 = millis();
    }
  }
  //gripper(release_angle);
  //delay(100);
  //gripper(release_angle+70);
  wrister(up_angle-70);
}
void Lineflower::out_restriction_zone(unsigned int cross_section_time)
{
  wrister(up_angle-10);
  gripper(grab_angle-20);
  delay(100);
  left_wheel(base_speed);
  right_wheel(-base_speed);
  delay(0);
  left_wheel(0);
  right_wheel(0);
  delay(200);
  unsigned long time1 = millis();
  int con = 0;
  while(true)
  {
	flow_lineb();
    if((millis()-time1)>cross_section_time&&(error1 <= -12 || error2 >= 12))
    {
		con++;
		//if(con == 2)//
		{
			left_wheel(0);
			right_wheel(0);
			break;
		}
    }
  }
  left_wheel(-base_speed);
  right_wheel(-base_speed);
  delay(700);
  left_wheel(0);
  right_wheel(0);
  delay(100);
  left_wheel(-(base_speed-20));
  right_wheel(base_speed);
  delay(500);
  left_wheel(0);
  right_wheel(0);
  delay(100);
  /*
  left_wheel(-base_speed);
  right_wheel(base_speed);
  delay(4.8*forward_time);
  left_wheel(0);
  right_wheel(0);*/
  left_turn(turn_speed,0,turn_time);
  left_turn(turn_speed,0,turn_time);
  current_direction = 270;
  /*
  left_wheel(-base_speed);
  right_wheel(-base_speed);
  delay(500);
  left_wheel(-base_speed);
  right_wheel(base_speed);
  delay(70);*/
  left_wheel(0);
  right_wheel(0);/*
  left_wheel(-base_speed);
  right_wheel(-base_speed);
  delay(600);
  left_wheel(0);
  right_wheel(0);
  delay(100);
  
  time1 = millis();
  while(true)
  {
	flow_line();
    if((millis()-time1)>300)//(error1 <= -12 && error2 >= 12)||((millis()-time1)>cross_section_time*1.7))//&&(millis()-time1)>cross_section_time)
    {
		left_wheel(0);
		right_wheel(0);
        break;
    }
  }////
  time1 = millis();
  while(true)
  {
	flow_line();
    if((millis()-time1)>170)//&&(millis()-time1)>cross_section_time)
    {
		left_wheel(0);
		right_wheel(0);
        break;
    }
  }*/
  gripper(release_angle+70);
}

void Lineflower::load_data()
{/*
  Serial.println("Max data:");
  for(int i = 0;i<20;i+=2)
  {
    sen_max_data[i]=int(EEPROM.read(i))|int(EEPROM.read(i+1))<<8;
	delay(10);
	Serial.print(sen_max_data[i]);
	Serial.print("\t");
  }
  Serial.println();
  Serial.println("Min Data:");
  for(int i= 0;i<10;i++)
  {
    sen_min_data[i] = int(EEPROM.read(i+20));
	delay(10);
	Serial.print(sen_min_data[i]);
	Serial.print("\t");
  }*/
  Serial.println();
  //counter = EEPROM.read(30);
  delay(10);
  block_base[0] = EEPROM.read(31);
  Serial.println();
  Serial.println("box1:");
  Serial.println(block_base[0]);
  delay(10);
  block_base[1] = EEPROM.read(32);
  delay(10);
  Serial.println();
  Serial.println("box2:");
  Serial.println(block_base[1]);
  for(int i = 0;i < game_field->r*game_field->c;i++)
  {
    game_field->board[i] = EEPROM.read(50+i);
	delay(10);
  }
  game_field->init_grid();
  game_field->print_board();
  check_point = EEPROM.read(100);
  wait_for_permission = true;
  if(check_point == 2)
  {
	current_direction = 0;
  }
  else if(check_point == 3)
  {
	current_direction = 270;
  }
  else if(check_point == 5)
  {
	check_point = 3;
	current_direction = 270;
	wait_for_permission = false;
  }
  Serial.println();
  Serial.println("check_point:");
  Serial.println(check_point);
  Serial.println("allign_value");
  
  		allign_value[0]=EEPROM.read(110)-10;
		delay(10);
		Serial.println(allign_value[0]);
		allign_value[1]=EEPROM.read(111)-10;
		delay(10);
		Serial.println(allign_value[1]);
		allign_value[2]=EEPROM.read(112)-10;
		delay(10);
		Serial.println(allign_value[2]);
		allign_value[3]=EEPROM.read(113)-10;
		delay(10);
		Serial.println(allign_value[3]);
		allign_value[4] =EEPROM.read(114)-10;
		delay(10);
		Serial.println(allign_value[4]);
		byte len=0;
		path1="";
		len=EEPROM.read(120);
		Serial.println(len);
		delay(10);
		int mem = 120+len;
		for(int i = 0;i<len;i++)
		{
			int val=EEPROM.read(121+i);
			path1+=val==76?"L":val==83?"S":"R";
			delay(10);
		}
		
		path2="";
		mem=mem+3;
		len=EEPROM.read(mem);
		Serial.println(len);
		delay(10);
		mem=mem+1;
		for(int i = 0;i<len;i++)
		{
			int val=EEPROM.read(mem+i);
			path2+=val==76?"L":val==83?"S":"R";
			delay(10);
		}
		mem+=len;
		
		
		path3="";
		mem=mem+3;
		len=EEPROM.read(mem);
		Serial.println(len);
		delay(10);
		mem=mem+1;
		for(int i = 0;i<len;i++)
		{
		    int val=EEPROM.read(mem+i);
			path3+=val==76?"L":val==83?"S":"R";
			delay(10);
		}
		mem+=len;
		
		
		path4="";
		mem=mem+3;
		len=EEPROM.read(mem);
		Serial.println(len);
		delay(10);
		mem=mem+1;
		for(int i = 0;i<len;i++)
		{
			int val=EEPROM.read(mem+i);
			path4+=val==76?"L":val==83?"S":"R";
			delay(10);
		}
		mem+=len;
		
		path5="";
		mem=mem+3;
		len=EEPROM.read(mem);
		Serial.println(len);
		delay(10);
		mem=mem+1;
		for(int i = 0;i<len;i++)
		{
			int val=EEPROM.read(mem+i);
			path5+=val==76?"L":val==83?"S":"R";
			delay(10);
		}
		Serial.println(path1);
		Serial.println(path2);
		Serial.println(path3);
		Serial.println(path4);
		Serial.println(path5);
		byte val = EEPROM.read(250);
		if(val == 1)
		{
			wait_for_big_bro = false;
		}
		blow_buzzer();
}

void Lineflower::drop_box(unsigned int cross_time)
{///*
  if(block_base[1]==45)
  {
	box_node = true;
  }
  rotate_bot_to_given_position(0,turn_speed,forward_time,turn_time);
  box_node = false;//*/
  unsigned long time1 = millis();
  int cont = 0;
  while(true)
  {
    flow_line();
    if(cont == 2)
    {
      left_wheel(0);
      right_wheel(0);/*//
      wrister(down_angle);
      gripper(release_angle);
      delay(100);
	  
      while(box_range() < 210)
      {
        flow_line();
      }
	  left_wheel(0);
      right_wheel(0);
	  delay(300);
	  wrister(down_angle+15);
      gripper(grab_angle);
      wrister(up_angle-10);
	  delay(500);//*/
      time1 = millis();
	  base_speed += 5;
	  max_speed +=5;
	  min_speed -=5;
      while((millis()-time1) < box_pit_time+250)
      {
        flow_line();
      }
      left_wheel(0);
      right_wheel(0);
	  delay(500);
	  //left_wheel(turn_speed);
      //right_wheel(-turn_speed);
	  delay(0);
	  left_wheel(0);
      right_wheel(0);
	  delay(500);
	  ///
	  wrister(up_angle-110);
	  delay(100);
	  //
	  left_wheel(-base_speed);
	  right_wheel(-base_speed);
	  delay(150);
	  left_wheel(-base_speed);
	  right_wheel(base_speed);
	  delay(100);
	  left_wheel(base_speed);
	  right_wheel(-base_speed);
	  delay(100);
	  left_wheel(-base_speed);
	  right_wheel(base_speed);
	  delay(100);
	  left_wheel(base_speed);
	  right_wheel(-base_speed);
	  delay(100);
	  left_wheel(0);
	  right_wheel(0);
	  
	  //
      gripper(release_angle);
	  delay(100);
	  wrister(up_angle);
	  delay(100);
      gripper(grab_angle-20);
	  delay(100);
      wrister(down_angle+10);
      delay(300);
      wrister(up_angle-10);
	  delay(300);
	  while(box_range() > 200);
	  delay(7000);
	  /*///
	  if(box_range() > 210)
	  {
	  ///
	  gripper(release_angle);
	  wrister(down_angle+30);
	  gripper(grab_angle);
	  left_wheel(base_speed);
	  right_wheel(base_speed);
	  delay(80);
	  left_wheel(-base_speed);
	  right_wheel(-base_speed);
	  delay(80);
	  left_wheel(-base_speed);
	  right_wheel(base_speed);
	  delay(80);
	  left_wheel(base_speed);
	  right_wheel(-base_speed);
	  delay(80);
	  left_wheel(0);
	  right_wheel(0);
	  delay(100);
      goto lebl;	  
	  }
	  ///*/
      cont = 3;
      time1 = millis();
	  if(avoid_pit_flag)
	  {
		avoid_pit();
	  }
	  time1 = millis();
	  
	  //return;
    }
    if(cont == 3 && (millis()-time1)<last_flow_line_time)
    {
      continue;
    }
    else if(cont == 3)
    {
      left_wheel(0);
      right_wheel(0);
      break;
    }

    if((millis()-time1)>cross_time&&(error1 <= -12 && error2 >= 12))
    {
      cont++;
	  //cont+=2;
      if(cont == 1)
      {
        left_turn(turn_speed,forward_time,turn_time);
		time1 = millis();
		while((millis()-time1)<50)
		{
			flow_line();
		}
		left_wheel(-turn_speed);
		right_wheel(turn_speed);
		delay(50);
		left_wheel(0);
		right_wheel(0);
		//
		left_wheel(-base_speed);
		right_wheel(-base_speed);
		delay(400);
		//
		if(true)//wait_for_permission)
		{
			take_permission_from_big_bro();
		}
		///
		left_wheel(0);
		right_wheel(0);
		wrister(down_angle);
		gripper(release_angle);
		delay(100);
	  
		while(box_range() < 210)
		{
			flow_line();
		}
		left_wheel(0);
		right_wheel(0);
		delay(300);
		wrister(down_angle+15);
		gripper(grab_angle);
		wrister(up_angle-10);
		delay(500);
		///
      }
      time1 = millis();
    }
  }
}

void Lineflower::find_path_and_save()
{
		EEPROM.write(250,0);
		delay(100);
        game_field->set_node(block_base[1]);
        game_field->set_node(40);
        game_field->set_source(1);
        game_field->set_destination(block_base[0]);
        path1=game_field->get_path();
		allign_value[0] = game_field->que[0]-game_field->que[1]-1+10;
        
		game_field->remove_node(40);
        game_field->set_source(block_base[0]);
        game_field->set_destination(40); 
		path2=game_field->get_path();
		allign_value[1] = game_field->que[0]-game_field->que[1]-1+10;
        
		game_field->set_node(block_base[0]);
        game_field->set_node(block_base[1]);
        game_field->set_source(40);
        game_field->set_destination(44);
		path3=game_field->get_path();
        allign_value[2] = game_field->que[0]-game_field->que[1]-1+10;
		
        game_field->remove_node(block_base[1]);
        game_field->set_source(44);
        game_field->set_destination(block_base[1]);
        path4=game_field->get_path();
        allign_value[3] = game_field->que[0]-game_field->que[1]-1+10;
		
        game_field->set_source(block_base[1]);
        game_field->set_destination(45);
		path5=game_field->get_path();
        allign_value[4] = game_field->que[0]-game_field->que[1]-1+10;
		
		EEPROM.write(110,allign_value[0]);
		delay(10);
		EEPROM.write(111,allign_value[1]);
		delay(10);
		EEPROM.write(112,allign_value[2]);
		delay(10);
		EEPROM.write(113,allign_value[3]);
		delay(10);
		EEPROM.write(114,allign_value[4]);
		byte len=path1.length();
		EEPROM.write(120,len);
		delay(10);
		int mem = 120+len;
		for(int i = 0;i<len;i++)
		{
			EEPROM.write(121+i,path1.charAt(i));
			delay(10);
		}
		
		len=path2.length();
		mem=mem+3;
		EEPROM.write(mem,len);
		delay(10);
		mem=mem+1;
		for(int i = 0;i<len;i++)
		{
			EEPROM.write(mem+i,path2.charAt(i));
			delay(10);
		}
		mem+=len;
		
		len=path3.length();
		mem=mem+3;
		EEPROM.write(mem,len);
		mem=mem+1;
		delay(10);
		for(int i = 0;i<len;i++)
		{
			EEPROM.write(mem+i,path3.charAt(i));
			delay(10);
		}
		mem+=len;
		
		len=path4.length();
		mem=mem+3;
		EEPROM.write(mem,len);
		delay(10);
		mem=mem+1;
		for(int i = 0;i<len;i++)
		{
			EEPROM.write(mem+i,path4.charAt(i));
			delay(10);
		}
		mem+=len;
		
		len=path5.length();
		mem=mem+3;
		EEPROM.write(mem,len);
		delay(10);
		mem=mem+1;
		for(int i = 0;i<len;i++)
		{
			EEPROM.write(mem+i,path5.charAt(i));
			delay(10);
		}
		Serial.println("original path");
		Serial.println(path1);
		Serial.println(path2);
		Serial.println(path3);
		Serial.println(path4);
		Serial.println(path5);
		Serial.println("original path");
		blow_buzzer();
}

void Lineflower::avoid_pit()
{
	left_wheel(-turn_speed);
	right_wheel(turn_speed);
	delay(500);
	left_wheel(0);
	right_wheel(0);
	delay(200);
	
	left_wheel(base_speed);
	right_wheel(base_speed);
	delay(6*forward_time);
	left_wheel(0);
	right_wheel(0);
	delay(200);
	
	left_wheel(turn_speed);
	right_wheel(-turn_speed);
	delay(1000);
	left_wheel(0);
	right_wheel(0);
	delay(200);
	
	left_wheel(base_speed);
	right_wheel(base_speed);
	delay(4*forward_time);
	get_error();
	while(error1>=-3)
	{
		get_error();
	}
	delay(100);
	left_wheel(0);
	right_wheel(0);
	delay(200);
	left_wheel(-(base_speed-30));
	right_wheel(base_speed);
	delay(270);
	left_wheel(0);
	right_wheel(0);
	delay(200);
}

