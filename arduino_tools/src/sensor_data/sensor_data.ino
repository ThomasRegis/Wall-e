#include "mpu6050.h"
#include <QMC5883L.h>
#include <Wire.h>

int increment = 0;
int magnX,magnY,magnZ;
int magnX0,magnY0,magnZ0;
const int mpu_adress = 0x68;
const int power = 0x00;
const int accelerometer_adress = 0x3B;
const int gyrometer_adress = 0x43;
const int gyro_calib = 500;
float time;
float delta_time;
Mpu6050 mpu(mpu_adress,power,gyrometer_adress,accelerometer_adress);
QMC5883L compass;
float current_time;
void setup(){
  // put your setup code here, to run once:
  //Wire.begin();
  Serial.begin(9600); ///20 Hz);
  mpu.setup();
  mpu.gyroCalibration(gyro_calib);
  compass.init();
  //compass.read(&magnX0,&magnY0,&magnZ0);  
}

void loop(){
  delay(200); ///20 Hz
  delta_time = millis() - time;
  time = millis();
  if(increment ==0){
    compass.read(&magnX0,&magnY0,&magnZ0);
    increment+=1;
  }
  compass.read(&magnX,&magnY,&magnZ);
  mpu.gyroData();
  mpu.accData();
  Serial.print(magnX);
  Serial.print(","); 
  Serial.print(magnY);
  Serial.print(","); 
  Serial.print(magnZ);
  Serial.print(","); 
  Serial.print(mpu._gyroX);
  Serial.print(","); 
  Serial.print(mpu._gyroY);
  Serial.print(","); 
  Serial.print(mpu._gyroZ);
  Serial.print(","); 
  Serial.print(mpu._accX);
  Serial.print(",");
  Serial.print(mpu._accY);
  Serial.print(","); 
  Serial.print(mpu._accZ);
  Serial.print(",");
  Serial.print(delta_time/1000);
  Serial.print(",");
  Serial.print(magnX0);
  Serial.print(","); 
  Serial.print(magnY0);
  Serial.print(","); 
  Serial.print(magnZ0);
  Serial.println();
}

// #include "mpu6050.h"

// const int mpu_adress = 0x68;
// const int power = 0x00;
// const int accelerometer_adress = 0x3B;
// const int gyrometer_adress = 0x43;

// Mpu6050 mpu(mpu_adress,power,gyrometer_adress,accelerometer_adress);

// void setup(){
//   // put your setup code here, to run once:
//   //Wire.begin();
//   Serial.begin(9600);
//   mpu.setup();
// }

// void loop(){
//   delay(100); ///20 Hz
//   mpu.accData(); 
//   Serial.print(mpu._accX);
//   Serial.print(",");
//   Serial.print(mpu._accY);
//   Serial.print(","); 
//   Serial.print(mpu._accZ);
//   Serial.print(",");
//   Serial.println();
// }


// #include <QMC5883L.h>

// int magnX,magnY,magnZ;
// QMC5883L compass;
// void setup(){
//   // put your setup code here, to run once:
//   Wire.begin();
//   Serial.begin(9600);
//   compass.init();
// }

// void loop(){
//   delay(100); ///20 Hz
//   compass.read(&magnX,&magnY,&magnZ);
//   Serial.print(magnX);
//   Serial.print(","); 
//   Serial.print(magnY);
//   Serial.print(","); 
//   Serial.print(magnZ);
//   Serial.print(",");
//   Serial.println();
// }



