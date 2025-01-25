#include "mpu6050.h"

Mpu6050::Mpu6050(const int i2c_adress, const int pwr_register,const int gyro_adress,const int acc_adress){
  _i2c_adress = i2c_adress;
  _pwr_register = pwr_register;
  _acc_adress = acc_adress;
  _gyro_adress = gyro_adress;
}

void Mpu6050::setup(){
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(_i2c_adress);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(_pwr_register);                 // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
}

void Mpu6050::gyroCalibration(const int number_values){
  int c = 0;
  while (c < number_values){
    Wire.beginTransmission(_i2c_adress);
    Wire.write(_gyro_adress);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2c_adress, 6, true);
    float x = Wire.read() << 8 | Wire.read();
    float y = Wire.read() << 8 | Wire.read();
    float z = Wire.read() << 8 | Wire.read();
    // Sum all readings
    _gyroX_error +=  x/ 131.0;
    _gyroY_error +=  y/ 131.0;
    _gyroZ_error +=  z/ 131.0;
   
    c++;
}
  //Divide the sum by 200 to get the error value
  _gyroX_error /= number_values;
  _gyroY_error /= number_values;
  _gyroZ_error /= number_values;
}

void Mpu6050::gyroData(){
  Wire.beginTransmission(_i2c_adress);
    Wire.write(_gyro_adress);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2c_adress, 6, true);
    _gyroX = (Wire.read() << 8 | Wire.read())/(131.0) - _gyroX_error;
    _gyroY = (Wire.read() << 8 | Wire.read())/(131.0) - _gyroY_error;
    _gyroZ = (Wire.read() << 8 | Wire.read())/(131.0) - _gyroZ_error;
}

void Mpu6050::accData(){
  Wire.beginTransmission(_i2c_adress);
    Wire.write(_acc_adress);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2c_adress, 6, true);
    _accX = (Wire.read() << 8 | Wire.read())/16384.0;
    _accY = (Wire.read() << 8 | Wire.read())/16384.0;
    _accZ = (Wire.read() << 8 | Wire.read())/16384.0;
}
