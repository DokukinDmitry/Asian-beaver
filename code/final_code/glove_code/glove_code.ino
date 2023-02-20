/* **********************************************************************************
Автор: Докукин Дмитрий
Ссылка на github: https://github.com/DokukinDmitry/Asian-beaver
Дата: 14.02.2023
Описание: Роботизированная перчатка (КМ) для управления манипулятором с помощью Bluetooth.
*********************************************************************************** */  

#include <SoftwareSerial.h>
#include <Wire.h>

// Подключение  

// Создаем переменные для кнопок
int pinkie_1 = 0; // кнопка на мизинце 1
int pinkie_2 = 0; // кнопка на мизинце 2
int finger_1 = 0; // кнопка на указательном пальце 1
int finger_2 = 0; // кнопка на указательном пальце 2
int thumb_1 = 0; // кнопка на запястье пальце 1
int thumb_2 = 0; // кнопка на запястье пальце 2

// Задаем порты для кнопок
int pinkie_Data_1 = 7;
int pinkie_Data_2 = 8;
int finger_Data_1 = 5;
int finger_Data_2 = 6;
int thumb_Data_1 = 4;
int thumb_Data_2 = 3;

// const int MPU_addr = 0x68;
const int MPU2 = 0x69, MPU1 = 0x68;

// Первый MPU6050
int16_t AcX1, AcY1, AcZ1, Tmp1, GyX1, GyY1, GyZ1;
int minVal = 265;
int maxVal = 402;
double x;
double y;
double z;

// Второй   MPU6050
int16_t AcX2, AcY2, AcZ2, Tmp2, GyX2, GyY2, GyZ2;
int minVal2 = 265;
int maxVal2 = 402;
double x2;
double y2;
double z2;

// Как часто отправлять значения манипулятору робота
int response_time = 100;

void setup() {
  pinMode(pinkie_Data_1, INPUT_PULLUP);
  pinMode(pinkie_Data_2, INPUT_PULLUP);
  pinMode(finger_Data_1, INPUT_PULLUP);
  pinMode(finger_Data_2, INPUT_PULLUP);
  pinMode(thumb_Data_1, INPUT_PULLUP);
  pinMode(thumb_Data_2, INPUT_PULLUP);

  Wire.begin();
  Wire.beginTransmission(MPU1);
  Wire.write(0x6B);// PWR_MGMT_1 регистр
  Wire.write(0); // установить в ноль (пробуждает MPU-6050)
  Wire.endTransmission(true); Wire.begin();
  Wire.beginTransmission(MPU2);
  Wire.write(0x6B);// PWR_MGMT_1 регистр
  Wire.write(0); // установить в ноль (пробуждает MPU-6050
  Wire.endTransmission(true);
  Serial.begin(4800);
  delay(1000);

}
void loop() {

  // получаем значения для первого микропроцессора с адресом 0x68
  GetMpuValue1(MPU1);
  //Serial.prinlnt("  ");
  delay(10);

  // получаем значения для второго процессора с адресом 0x69
  GetMpuValue2(MPU2);
  //Serial.println("");
  delay(10);
  
  // Распечатать значение, основанное на изменении координат XYZ 1-го или 2-го MPU

  // Двигаться влево
  if ( x > 15 && x < 55 && y < 30) {
    Serial.print("L");
    delay(response_time);
  }

  // Двигаемся вправо
  if ( x < 310 && x > 270) {
    Serial.print("R");
    delay(response_time);
  }

  // Гриппер вверх
  if ( y > 60 && y < 80) {
    Serial.print("G");
    delay(response_time);
  }

  // Гриппер вниз
  if ( y < 310 && y > 270) {
    Serial.print("U");
    delay(response_time);
  }

  // Двигаемся вправо
  if ( y2 > 50 && y2 < 85) {
    Serial.print("C");
    delay(response_time);
  }

  // Движение влево --- Правая рука
  if ( y2 < 160 && y2 > 120) {
    Serial.print("c");
    delay(response_time);

  }

  // считываем значения с кнопок в Arduino
  pinkie_1 = digitalRead(pinkie_Data_1);
  pinkie_2 = digitalRead(pinkie_Data_2);
  finger_1 = digitalRead(finger_Data_1);
  finger_2 = digitalRead(finger_Data_2);
  thumb_1 = digitalRead(thumb_Data_1);
  thumb_2 = digitalRead(thumb_Data_2);

  // Мизинец 
  if (pinkie_1 == LOW) {
    Serial.print("P");
    delay(response_time);

  }
  if (pinkie_2 == LOW) {
    Serial.print("p");
    delay(response_time);
  }


  // запястье 1 - запястье (вращение базы)
  if (thumb_1 == LOW) {
    Serial.print("T");
    delay(response_time);
  }

  if (thumb_2 == LOW) {
    Serial.print("t");
    delay(response_time);
  }

  // указательный палец 1 - клешня закрыть/открыть
  if (finger_1 == LOW) {
    Serial.print("F");
    delay(response_time);
  }

  if (finger_2 == LOW) {
    Serial.print("f");
    delay(response_time);
  }
  else {
    delay(5);
  }
}

void GetMpuValue1(const int MPU) {

  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // начиная с регистра 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); // запрашиваем всего 14 регистров

  AcX1 = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY1 = Wire.read() << 8 |  Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ1 = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  Tmp1 = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

  int xAng = map(AcX1, minVal, maxVal, -90, 90);
  int yAng = map(AcY1, minVal, maxVal, -90, 90);
  int zAng = map(AcZ1, minVal, maxVal, -90, 90);

  GyX1 = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY1 = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ1 = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI) + 4; //смещение на 4 градуса, чтобы вернуться к нулю
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  // -- Комментарий для отладки первого MPU
  //  Serial.print("AngleX= ");
  //  Serial.print(x);
  //  Serial.print("\t");
  
  //  Serial.print("AngleY= ");
  //  Serial.print(y);
  //  Serial.print("\t");
  
  //  Serial.print("AngleZ= ");
  //  Serial.print(z);
  //  Serial.print("\t");
  //  Serial.println("-----------------------------------------");


  //  Serial.print("AcX = ");
  //  Serial.print(AcX1);
  //  Serial.print(" | AcY = ");
  //  Serial.print(AcY1);
  //  Serial.print(" | AcZ = ");
  //  Serial.print(AcZ1);
  //  Serial.print(" | GyX = ");
  //  Serial.print(GyX1);
  //  Serial.print(" | GyY = ");
  //  Serial.print(GyY1);
  //  Serial.print(" | GyZ = ");
  //  Serial.println(GyZ1);
}

void GetMpuValue2(const int MPU) {

  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // начиная с регистра 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); // запрашиваем всего 14 регистров
  AcX2 = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY2 = Wire.read() << 8 |  Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ2 = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  Tmp2 = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

  int xAng2 = map(AcX2, minVal2, maxVal2, -90, 90);
  int yAng2 = map(AcY2, minVal2, maxVal2, -90, 90);
  int zAng2 = map(AcZ2, minVal2, maxVal2, -90, 90);

  GyX2 = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY2 = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ2 = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  x2 = RAD_TO_DEG * (atan2(-yAng2, -zAng2) + PI) + 4; // смещение на 4 градуса, чтобы вернуться к нулю
  y2 = RAD_TO_DEG * (atan2(-xAng2, -zAng2) + PI);
  z2 = RAD_TO_DEG * (atan2(-yAng2, -xAng2) + PI);

  //    -- Комментарий для отладки второго MPU
     Serial.print("AcX = ");
     Serial.print(AcX2);
     Serial.print(" | AcY = ");
     Serial.print(AcY2);
     Serial.print(" | AcZ = ");
     Serial.print(AcZ2);
     Serial.print(" | GyX = ");
     Serial.print(GyX2);
     Serial.print(" | GyY = ");
     Serial.print(GyY2);
     Serial.print(" | GyZ = ");
     Serial.println(GyZ2);
  
     Serial.print("AngleX2= ");
     Serial.print(x2);
     Serial.print("\t");
  
     Serial.print("AngleY2= ");
     Serial.print(y2);
     Serial.print("\t");
  
     Serial.print("AngleZ2= ");
     Serial.print(z2);
     Serial.print("\t");
   Serial.println("-----------------------------------------");

}
