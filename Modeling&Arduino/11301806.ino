#include<Wire.h>
#include <SoftwareSerial.h> 
const int MPU_ADDR = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

double angleAcX, angleAcY, angleAcZ;

const double RADIAN_TO_DEGREE = 180 / 3.14159;
int Time=0;
int abAngle1 = 0;
int abAngle2 = 0;
int rx = 4;
int tx = 3;
int DIR1 = 6;
int PWM1 = 7;
int DIR2 = 11;
int PWM2 = 12;
int start = 999;
int start2 = 999;
int mainstart = 999;
int start3 = 999;
char bt2;
long bt; 
long userAngle = -5;
long userSideAngle= 999;
int obAngle = 0;
int degree = 0;
long ADCvalue1 = 0;
long ADCvalue2 = 0;
long angle1 = 0;
long angle2 = 0;
long ADCmin = 104;
long ADCmax = 919;
char mdir;
int mdir2;
int ended=999;
int moblie = 0;

SoftwareSerial mySerial(tx,rx);

void setup() {
  initSensor();
  DLPF();
  Serial.begin(9600);

  mySerial.begin(9600);
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH);
  ADCvalue1 = analogRead(A0);
  angle1 = (ADCvalue1-ADCmin)*360/(ADCmax-ADCmin); 
}

void loop() {
   if(Serial.available()){
   mySerial.write(Serial.read());
   
   }
  
      if (mySerial.available()) {
     
      //bt = mySerial.read();
      bt = mySerial.parseInt();
      
      Serial.print("사용자 입력 데이터:");
      Serial.print(bt);
      Serial.print("\n");
     if(bt>0){
      if (bt!=8756) {
        if(bt>=0){
          while (mainstart==999) {
              if(start == 999){
              angleSensor();
              userAngle = bt;
              delay(1500);
              userSideAngle = mySerial.parseInt();
              delay(100);
              moblie = 8888;
               mySerial.write(moblie);
              delay(1500);
              
 
              Serial.print("시간 설정값: ");
              Serial.println(userAngle);
              Serial.println(bt);
              Serial.print("강도 설정값: ");
              Serial.println(userSideAngle);
              delay(50);
              start = 0;

              obAngle = userSideAngle*8+5;
              //userAngle=userAngle;
              Serial.print("각도 임계: ");
              Serial.println(obAngle);
  
              
              
              }
     //
              if(isnan(angleAcY)){    //갑자기 멈출때 해결하는코드
              sensorRestart();
              }

              
              while(start2== 999) {
           
                  mySerial.print(moblie);
                 
                 angleSensor();
                 abAngle1=abs(angleAcX);
                 abAngle2=abs(angleAcY);
                 
                 Time= Time + 1;
                 Serial.print(Time);
                 delay(1000);
              
                 if(abAngle1>obAngle || abAngle2>obAngle  ){
                  Time = 0;
                  moblie = 8888;
                  Serial.print("   ");
                  Serial.print(moblie);
                  Serial.print("   ");
                  mySerial.print(moblie);
                  }

                 
                 if(Time>userAngle ){
                  Serial.println("종료됨");              
                  bt = NULL;
                  int at = sizeof(bt);
                  start = 999;
                  Serial.print(bt);
                  Serial.print(at);  
                  userAngle = NULL;
                  mainstart = 999;
                  start2 = 999;
                  ended=400;
                  break;
                 } 
                }
                 
                 if(ended==400){
                 delay(1000);
                 Serial.print("종료됨2");
                  moblie = 6666;
                  Serial.print("   ");
                  Serial.print(moblie);
    
                  mySerial.print(moblie);
                 ended=999;
                 Time=0;
                 break; 
                 
                 }
                
              }  
        }
 }
 
}
      }
}

void  angleSensor() {
  getData();
  angleAcY = atan(-AcX / sqrt(pow(AcY, 2) + pow(AcZ, 2)));
  angleAcY *= RADIAN_TO_DEGREE;
  angleAcX = atan(AcY / sqrt(pow(AcX, 2) + pow(AcZ, 2)));
  angleAcX *= RADIAN_TO_DEGREE;
  
  Serial.print("수직각도:");
  Serial.print(angleAcY);
  Serial.print("    수평각도:");
  Serial.println(angleAcX);
  delay(333);
  
}

void sensorRestart() {   
  Serial.println("quit");
  initSensor();
  angleSensor();
}
  


  
void initSensor() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void DLPF() {  
  Wire.beginTransmission(MPU_ADDR); //DLPF조정
  Wire.write(0x1A);
  Wire.write(0x03);
  Wire.endTransmission(true);  
}

void getData() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}
