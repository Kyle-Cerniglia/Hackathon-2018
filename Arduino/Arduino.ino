#include <Wire.h>
#include <LSM6.h>

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
  Wire.begin();
  /*
  if (!imu.init()){
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  imu.enableDefault();
  */
}

void loop() {
  drive_stop();
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

