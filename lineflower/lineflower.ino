
int base_speed = 130;
int max_speed = 250;
int min_speed= -250;

int error = 0;
int error1 = 0;
int error2 = 0;
int kp = 15;

int sensor_threshold_value = 85;
int error_call_counter = 0;
char current_turn = 's';

int sonor_trigger_pin = 35;
int sonor_vcc = 33;
int sonor_gnd = 39;
int sonor_input_pin =37; 

int sonor_l_tp =A1;
int sonor_l_ip =A2;
int sonor_l_vcc =A0;
int sonor_l_gnd =A3;

int btn1 = 11;
int btn2 = 10;
int btn3 = 8;
int btn4 = 9;

int led = 13;

int left_m_f = 6;
int left_m_b = 7;
int right_m_f = 5;
int right_m_b = 4;
int no_of_sensor = 10;
int sensor_input_pin[]={
  6,7,8,9,10,11,12,13,14,15};
int sen_max_data[10] = {
  0,0,0,0,0,0,0,0,0,0};
int sen_min_data[10] = {
  1023,1023,1023,1023,1023,1023,1023,1023,1023,1023};

void setup()
{

  pinMode(sonor_vcc,OUTPUT);
  digitalWrite(sonor_vcc,HIGH);
  pinMode(sonor_gnd,OUTPUT);
  digitalWrite(sonor_gnd,LOW);
  pinMode(sonor_trigger_pin,OUTPUT);
  pinMode(sonor_input_pin,INPUT);

  pinMode(sonor_l_tp,OUTPUT);
  pinMode(sonor_l_ip,INPUT);
  pinMode(sonor_l_vcc,OUTPUT);
  pinMode(sonor_l_gnd,OUTPUT);
  digitalWrite(sonor_l_vcc,HIGH);
  digitalWrite(sonor_l_gnd,LOW);


  pinMode(led,OUTPUT);

  pinMode(btn1,INPUT);
  pinMode(btn2,INPUT);
  pinMode(btn3,INPUT);
  pinMode(btn4,INPUT);

  pinMode(left_m_f,OUTPUT);
  pinMode(left_m_b,OUTPUT);
  pinMode(right_m_f,OUTPUT);
  pinMode(right_m_b,OUTPUT);

  Serial.begin(9600);
}
void loop()
{ 
  delay(100);
}

int button_press_check()
{
  if(digitalRead(btn1) == LOW)
  {
    digitalWrite(led,HIGH);
    delay(200);
    digitalWrite(led,LOW);
    delay(200);
    return 1;
  }
  else if(digitalRead(btn2) == LOW)
  {
    digitalWrite(led,HIGH);
    delay(200);
    digitalWrite(led,LOW);
    delay(200);
    return 2;
  }
  else if(digitalRead(btn3) == LOW)
  {
    digitalWrite(led,HIGH);
    delay(200);
    digitalWrite(led,LOW);
    delay(200);
    return 3;
  }
  else if(digitalRead(btn4) == LOW)
  {
    digitalWrite(led,HIGH);
    delay(200);
    digitalWrite(led,LOW);
    delay(200);
    return 4;
  }
  return 0;
}
void read_sensor()
{
  for(int i = 0;i<no_of_sensor;i++)
  {
    Serial.println(analogRead(sensor_input_pin[i]));
  }
  Serial.println("##########################");
  delay(500);
}
void flow_line()
{
  get_error();
  int turn = kp*error;
  left_wheel(base_speed - turn);
  right_wheel(base_speed + turn);
}
void get_error()
{
  error1 = 0;
  error2 = 0;
  error = 0;
  int data = analogRead(sensor_input_pin[0]);
  error_call_counter++;

  data = analogRead(sensor_input_pin[0]);
  data = constrain(data,sen_min_data[0],sen_max_data[0]);
  data = map(data,sen_min_data[0],sen_max_data[0],0,256);
  error1 += data < sensor_threshold_value?-16:0;


  data = analogRead(sensor_input_pin[1]);
  data = constrain(data,sen_min_data[1],sen_max_data[1]);
  data = map(data,sen_min_data[1],sen_max_data[1],0,256);
  error1 += data < sensor_threshold_value?-6:0;

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
  error2 += data < sensor_threshold_value?6:0;

  data = analogRead(sensor_input_pin[9]);
  data = constrain(data,sen_min_data[9],sen_max_data[9]);
  data = map(data,sen_min_data[9],sen_max_data[9],0,256);
  error2 += data < sensor_threshold_value?16:0;

  error = error1 + error2;
}

void calibrate()
{
  unsigned long time = millis();
  for(int i=0;i<3;i++)
  {
    left_wheel(50);
    right_wheel(-50);
    time = millis();
    while((millis() - time) < 1000)
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
    left_wheel(-50);
    right_wheel(50);
    while((millis() - time) < 1050)
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
  for(int i = 0;i<10;i++)
  {
    Serial.print(sen_max_data[i]);
    Serial.print("    ");
  }
  Serial.println("\nmin sensor data");
  for(int i= 0;i<10;i++)
  {
    Serial.print(sen_min_data[i]);
    Serial.print("    ");
  }
}

void left_wheel(int spd)
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
void right_wheel(int spd)
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
double get_distance()
{
  digitalWrite(sonor_trigger_pin,HIGH);
  delayMicroseconds(10);
  digitalWrite(sonor_trigger_pin,LOW);
  int i = pulseIn(sonor_input_pin,HIGH);
  double distance = 0.017 * i;//distance in cm
  return distance;
}

double get_distance_l()
{
  digitalWrite(sonor_l_tp,HIGH);
  delayMicroseconds(10);
  digitalWrite(sonor_l_tp,LOW);
  int i = pulseIn(sonor_l_ip,HIGH);
  double distance = 0.017 * i;//distance in cm
  return distance;
}






















