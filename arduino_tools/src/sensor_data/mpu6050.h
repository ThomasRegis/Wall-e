#include <Wire.h>

class Mpu6050{
  public :
    Mpu6050(const int i2c_adress, const int pwr_register,const int gyro_adress,const int acc_adress);
    void setup();
    void gyroCalibration(const int number_values);
    void gyroData();
    void accData();

    float _gyroX;
      float _gyroY;
      float _gyroZ;

      float _accX;
      float _accY;
      float _accZ;
  
  private :
      
      int _i2c_adress;
      int _pwr_register;
      int _gyro_adress;
      int _acc_adress;
      
      float _gyroX_error;
      float _gyroY_error;
      float _gyroZ_error;
};