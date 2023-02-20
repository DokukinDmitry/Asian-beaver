#include "HCPCA9685.h"
#define  I2CAdd 0x40

HCPCA9685 HCPCA9685(I2CAdd);
 
void setup(){
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);
  HCPCA9685.Servo(0, 10);
    HCPCA9685.Servo(1, 10);
    HCPCA9685.Servo(2, 10);
    HCPCA9685.Servo(3, 10);
    HCPCA9685.Servo(4, 10);
    HCPCA9685.Servo(5, 10);
    delay(100);
}
 
 
void loop(){
  unsigned int Pos;
  for(Pos = 10; Pos < 450; Pos=Pos+10){
    HCPCA9685.Servo(0, Pos);
    HCPCA9685.Servo(1, Pos);
    HCPCA9685.Servo(2, Pos);
    HCPCA9685.Servo(3, Pos);
    HCPCA9685.Servo(4, Pos);
    HCPCA9685.Servo(5, Pos);
    delay(15);
  }
 
  for(Pos = 450; Pos >= 10; Pos=Pos-10)
  {
    HCPCA9685.Servo(0, Pos);
    HCPCA9685.Servo(1, Pos);
    HCPCA9685.Servo(2, Pos);
    HCPCA9685.Servo(3, Pos);
    HCPCA9685.Servo(4, Pos);
    HCPCA9685.Servo(5, Pos);
    delay(15);
  }
}