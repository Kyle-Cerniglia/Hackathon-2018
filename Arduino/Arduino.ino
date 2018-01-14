#include <Wire.h>
#include <LSM6.h>
#include <String.h>
#include <stdio.h>

/*
The sensor outputs provided by the library are the raw
16-bit values obtained by concatenating the 8-bit high and
low accelerometer and gyro data registers. They can be
converted to units of g and dps (degrees per second) using
the conversion factors specified in the datasheet for your
particular device and full scale setting (gain).
Example: An LSM6DS33 gives an accelerometer Z axis reading
of 16276 with its default full scale setting of +/- 2 g. The
LA_So specification in the LSM6DS33 datasheet (page 15)
states a conversion factor of 0.061 mg/LSB (least
significant bit) at this FS setting, so the raw reading of
16276 corresponds to 16276 * 0.061 = 992.8 mg = 0.9928 g.
*/

#define MOTOR_LF 5
#define MOTOR_LB 11
#define MOTOR_RF 6
#define MOTOR_RB 9

#define IR_FL_TRIG 4
#define IR_FL_RANGE A0
#define IR_FC_TRIG 15
#define IR_FC_RANGE 8
#define IR_FR_TRIG 16
#define IR_FR_RANGE A1
#define IR_B_TRIG 7
#define IR_B_RANGE 10

int timeold = 0;
int timenew = 0;
int gyrooffset = 350;
double heading = 0;
double oldheading = 0;
char rxpacket[4];

LSM6 imu;

double IR_convert(int range);
double poll_heading();
void drive_stop();
void drive_forward(int speeed);
void drive_backwards(int speeed);
void drive_left(int speeed);
void drive_right(int speeed);
void drive_right_forward(int speeed);
void drive_right_backwards(int speeed);
void drive_left_forward(int speeed);
void drive_left_backwards(int speeed);
void drive_degrees(int speeed, int degree);
void packet_tx();
void packet_rx();
void command(int cmd, double degsec, int speeed);

void setup() {
  
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  pinMode(IR_B_TRIG, INPUT);
  pinMode(MOTOR_LB, OUTPUT);
  pinMode(IR_FC_RANGE, INPUT);
  pinMode(MOTOR_LF, OUTPUT);
  pinMode(MOTOR_RF, OUTPUT);
  pinMode(IR_FL_TRIG, INPUT);
  pinMode(MOTOR_RB, OUTPUT);
  pinMode(IR_FL_RANGE, INPUT);
  pinMode(IR_FR_RANGE, INPUT);
  pinMode(IR_FC_TRIG, INPUT);
  pinMode(IR_FR_TRIG, INPUT);
  pinMode(IR_B_RANGE, INPUT);
  pinMode(13, OUTPUT);

  digitalWrite(MOTOR_LB, LOW);
  digitalWrite(MOTOR_LF, LOW);
  digitalWrite(MOTOR_RF, LOW);
  digitalWrite(MOTOR_RB, LOW);
  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();
  
  if (!imu.init()){
    while (1);
  }
  imu.enableDefault();

}

void loop() {
  
  while(!Serial1.available()){
    poll_heading();
  }
  
  packet_rx();
}

double IR_convert(int range){//Converts IR sensor data into inches
  //Old
  //return 2.4 - 0.0085 * range + 0.000019 * pow(range, 2);//original 2.372275 - 0.0023251 * range + 0.0000185 * pow(range, 2); 2-4" 2.372275 - 0.0043251 * range + 0.000012 * pow(range, 2); 4-5" 2.0 - 0.005 * range + 0.000014 * pow(range, 2);
  if(range < 400){
    //1.5-3 (0-400)
    return 1.368299 + 0.0069761 * range - 0.0000073 * pow(range, 2);
  }
  else if(range >= 400 && range < 650){
    //3-4.5 (400-650)
    return 4.263307 - 0.0085843 * range + 0.0000137 * pow(range, 2);
  }
  else if(range >= 650 && range < 735){
    //4.5-6 (650-735)
    return 24.36777 - 0.0730488 * range + 0.0000654 * pow(range, 2);
  }
  else if(range >= 735 && range < 790){
    //6-7.5 (735-790)
    return 22.29654 - 0.0685216 * range + 0.0000631 * pow(range, 2);
  }
  else if(range >= 790 && range < 820){
    //7.5-9 (790-820)
    return 46.52758 - 0.1446143 * range + 0.0001205 * pow(range, 2);
  }
  else if(range >= 820 && range < 840){
    //9-10.5 (820-840)
    return 181.7581 - 0.4838064 * range + 0.0003331 * pow(range, 2);
  }
  else{
    //10.5-12 (840-860)
    return 1248.394 - 2.998878 * range + 0.0018157 * pow(range, 2);
  }
}

double poll_heading(){//Polls the IMU for rotational data
  timeold = timenew;
  timenew = (int)millis();
  imu.read();
  
  if(imu.g.z > 400 || imu.g.z < -400){
    oldheading = heading;
    heading += ((imu.g.z + gyrooffset) * 0.00875) * ((timenew - timeold) / 1000.0);
    if(abs(oldheading - heading) <= 0.01){
      heading = oldheading;
    }
  }
  if(heading > 180){
    heading = -(360 - heading);
  }
  else if(heading < -180){
    heading = 360 + heading;
  }
  return heading;
}

void drive_stop(){
  analogWrite(MOTOR_LF, 0);
  analogWrite(MOTOR_LB, 0);
  analogWrite(MOTOR_RF, 0);
  analogWrite(MOTOR_RB, 0);
}

void drive_forward(int speeed){
  analogWrite(MOTOR_LF, speeed);
  analogWrite(MOTOR_RF, speeed);
}

void drive_backwards(int speeed){
  analogWrite(MOTOR_LB, speeed);
  analogWrite(MOTOR_RB, speeed);
}

void drive_left(int speeed){
  analogWrite(MOTOR_LB, speeed);
  analogWrite(MOTOR_RF, speeed);
}

void drive_right(int speeed){
  analogWrite(MOTOR_LF, speeed);
  analogWrite(MOTOR_RB, speeed);
}

void drive_right_forward(int speeed){
  analogWrite(MOTOR_RF, speeed);
}

void drive_right_backwards(int speeed){
  analogWrite(MOTOR_RB, speeed);
}

void drive_left_forward(int speeed){
  analogWrite(MOTOR_LF, speeed);
}

void drive_left_backwards(int speeed){
  analogWrite(MOTOR_LB, speeed);
}

void drive_degrees(int speeed, int degree){
  int startheading = heading;
  int temp = 0;
  if(degree >= 0){
    drive_right(speeed);
    if(startheading + degree > 180){
      
      temp = -(360 - startheading - degree);
      while(heading < temp || heading > 0){
        poll_heading();
      }
    }
    else{
      
      while(heading < startheading + degree){
        //digitalWrite(13, HIGH);
        //Serial.println((int)heading);
        poll_heading();        
      }
    }
  }
  else{
    drive_left(speeed);
    if(startheading - degree < -180){
      temp = 360 + startheading - degree;
      while(heading > temp || heading < 0){
        poll_heading();
      }
    }
    while(heading > startheading + degree){
      poll_heading();
    }
  }
  drive_stop();
}

void packet_tx(){
  int temp = 0;
  Serial1.print("start,");
  temp = (int)(analogRead(IR_FC_RANGE) / 10);
  if(temp < 100){
    Serial1.print("0");
  }
  if(temp < 10){
    Serial1.print("0");
  }
  Serial1.print(temp);
  Serial1.print(",");
  temp = (int)(analogRead(IR_FL_RANGE) / 10);
  if(temp < 100){
    Serial1.print("0");
  }
  if(temp < 10){
    Serial1.print("0");
  }
  Serial1.print(temp);
  Serial1.print(",");
  temp = (int)(analogRead(IR_FR_RANGE) / 10);
  if(temp < 100){
    Serial1.print("0");
  }
  if(temp < 10){
    Serial1.print("0");
  }
  Serial1.print(temp);
  Serial1.print(",");
  temp = (int)(analogRead(IR_B_RANGE) / 10);
  if(temp < 100){
    Serial1.print("0");
  }
  if(temp < 10){
    Serial1.print("0");
  }
  Serial1.print(temp);
  Serial1.print(",");
  temp = (int)heading;
  if(temp >= 0){
    Serial1.print("+");
  }
  else{
    Serial1.print("-");
  }
  if(temp >= 0){
    if(temp < 100){
    Serial1.print("0");
    }
    if(temp < 10){
      Serial1.print("0");
    }
  }
  else{
    if(temp > -100){
      Serial1.print("0");
    }
    if(temp > -10){
      Serial1.print("0");
    }
  }
  Serial1.print(abs(temp));
  Serial1.print(",stop");  
}

void packet_rx(){
  
  int cmd = 0;
  double degsec = 0;
  int speeed = 0;
  //if(Serial1.available()){
    if(!strcmp(Serial1.read(),'~')){
      Serial.print("PING");
      //Serial1.readBytes(rxpacket, 4);
      //Serial.println(rxpacket);
      cmd = atoi(Serial1.read());
      /*
      Serial.println(rxpacket[0]);
      Serial.println(rxpacket[1]);
      Serial.println(rxpacket[2]);
      if(!strcmp(rxpacket[0], "0")){
        cmd = 0;
      }
      else if(!strcmp(rxpacket[0], "1")){
        cmd = 1;
      }
      else if(!strcmp(rxpacket[0], "2")){
        cmd = 2;
      }
      else if(!strcmp(rxpacket[0], "3")){
        cmd = 3;
      }
      else if(!strcmp(rxpacket[0], "4")){
        cmd = 4;
      }
      else if(!strcmp(rxpacket[0], "5")){
        cmd = 5;
      }
      else if(!strcmp(rxpacket[0], "6")){
        cmd = 6;
      }
      else if(!strcmp(rxpacket[0], "7")){
        cmd = 7;
      }
      else if(!strcmp(rxpacket[0], "8")){
        cmd = 8;
      }
      else{
        cmd = 9;
      }
      */
      Serial.print(":");
      Serial.print(cmd);
      degsec = atoi(Serial1.read()) * 10;
      /*
      if(!strcmp(rxpacket[1], '0')){
        degsec = 0;
      }
      else if(!strcmp(rxpacket[1], '1')){
        degsec = 10;
      }
      else if(!strcmp(rxpacket[1], '2')){
        degsec = 20;
      }
      else if(!strcmp(rxpacket[1], '3')){
        degsec = 30;
      }
      else if(!strcmp(rxpacket[1], '4')){
        degsec = 40;
      }
      else if(!strcmp(rxpacket[1], '5')){
        degsec = 50;
      }
      else if(!strcmp(rxpacket[1], '6')){
        degsec = 60;
      }
      else if(!strcmp(rxpacket[1], '7')){
        degsec = 70;
      }
      else if(!strcmp(rxpacket[1], '8')){
        degsec = 80;
      }
      else{
        degsec = 90;
      }
      */
      degsec += atoi(Serial1.read());
      /*
      if(!strcmp(rxpacket[2], '0')){
        degsec += 0;
      }
      else if(!strcmp(rxpacket[2], '1')){
        degsec += 1;
      }
      else if(!strcmp(rxpacket[2], '2')){
        degsec += 2;
      }
      else if(!strcmp(rxpacket[2], '3')){
        degsec += 3;
      }
      else if(!strcmp(rxpacket[2], '4')){
        degsec += 4;
      }
      else if(!strcmp(rxpacket[2], '5')){
        degsec += 5;
      }
      else if(!strcmp(rxpacket[2], '6')){
        degsec += 6;
      }
      else if(!strcmp(rxpacket[2], '7')){
        degsec += 7;
      }
      else if(!strcmp(rxpacket[2], '8')){
        degsec += 8;
      }
      else{
        degsec += 9;
      }
      */
      speeed = atoi(Serial1.read());
      /*
      if(!strcmp(rxpacket[3], '1')){
        speeed = 1;
      }
      else if(!strcmp(rxpacket[3], '2')){
        speeed = 2;
      }
      else if(!strcmp(rxpacket[3], '3')){
        speeed = 3;
      }
      else{
        speeed = 4;
      }
      */
      Serial.print(":");
      Serial.print(speeed);
      
      command(cmd, degsec, speeed);
    }
  //}  
}

void command(int cmd, double degsec, int speeed){
  switch(cmd){
    case 0://turn right
      drive_degrees(speeed * 64, degsec);
      break;

    case 3://turn left
      drive_degrees(speeed * 64, degsec);
      break;

     case 6://forward
      drive_forward(speeed * 64);
      delay(degsec * 100);
      drive_stop();
      break;

     case 7://backwards
      drive_backwards(speeed * 64);
      delay(degsec * 100);
      drive_stop();
      break;

     case 8://stop
      drive_stop();
      break;

     case 9://request
      packet_tx();
      break;

     default:
      break;
  }
  
}

