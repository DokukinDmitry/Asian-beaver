/* **************************************************************************************
Автор: Докукин Дмитрий
Ссылка на github: https://github.com/DokukinDmitry/Asian-beaver
Дата: 18.02.2023
Описание: Копирующий монипулятор защитных камер (УУКМ) для работы с  «горячими камерами».
***************************************************************************************** */

/* Подключение библиотеки HCPCA9685 */
#include "HCPCA9685.h"

/* Адрес ведомого устройства I2C для устройства/модуля. Для HCMODU0097 адрес I2C по умолчанию равен 0x40 */
#define  I2CAdd 0x40

/* Использование HCPCA9685 для настройки I2C */
HCPCA9685 HCPCA9685(I2CAdd);

// Исходное положение сервопривода
const int servo_joint_L_parking_pos = 60;
const int servo_joint_R_parking_pos = 60;
const int servo_joint_1_parking_pos = 70;
const int servo_joint_2_parking_pos = 47;
const int servo_joint_3_parking_pos = 63;
const int servo_joint_4_parking_pos = 63;

// Степень чувствительности сервопривода робота - Интервалы
int servo_joint_L_pos_increment = 20;
int servo_joint_R_pos_increment = 20;
int servo_joint_1_pos_increment = 20;
int servo_joint_2_pos_increment = 50;
int servo_joint_3_pos_increment = 60;
int servo_joint_4_pos_increment = 40;

// Текущее положение севопривода
int servo_joint_L_parking_pos_i = servo_joint_L_parking_pos;
int servo_joint_R_parking_pos_i = servo_joint_R_parking_pos;
int servo_joint_1_parking_pos_i = servo_joint_1_parking_pos;
int servo_joint_2_parking_pos_i = servo_joint_2_parking_pos;
int servo_joint_3_parking_pos_i = servo_joint_3_parking_pos;
int servo_joint_4_parking_pos_i = servo_joint_4_parking_pos;


// Минимальный и максимальный угол наклона сервопривода
int servo_joint_L_min_pos = 10;
int servo_joint_L_max_pos = 180;

int servo_joint_R_min_pos = 10;
int servo_joint_R_max_pos = 180;

int servo_joint_1_min_pos = 10;
int servo_joint_1_max_pos = 400;

int servo_joint_2_min_pos = 10;
int servo_joint_2_max_pos = 380;

int servo_joint_3_min_pos = 10;
int servo_joint_3_max_pos = 380;

int servo_joint_4_min_pos = 10;
int servo_joint_4_max_pos = 120;

int servo_L_pos = 0;
int servo_R_pos = 0;
int servo_joint_1_pos = 0;
int servo_joint_2_pos = 0;
int servo_joint_3_pos = 0;
int servo_joint_4_pos = 0;

char state = 0; // Изменяет значение с ASCII на char
int response_time = 5;
int response_time_4 = 2;
int loop_check = 0;
int response_time_fast = 20;
int action_delay = 600;

// Переменная для положения шагового мотора 
unsigned int Pos;

// Задаем порты для шагового мотора
const int dirPin = 4;
const int stepPin = 5;
const int stepsPerRevolution = 120;
int stepDelay = 4500;
const int stepsPerRevolutionSmall = 60;
int stepDelaySmall = 9500;

void setup()
{
  // Объявление пинов в качестве выходов
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  /* Инициация библиотеки для сервоприводов */
  HCPCA9685.Init(SERVO_MODE);

  /* Ввод устройства в рабочий режим */
  HCPCA9685.Sleep(false);

  Serial.begin(4800); // Скорость передачи данных по bluetooth


  delay(3000);
}


void loop() {

  if (Serial.available() > 0) { // Проверка поступления данных с серийного порта

    state = Serial.read(); // Считывание значений с серийного порта
    Serial.print(state); // Выводит полученные значения

    // Поворот базы монипулятора налево
    if (state == 'S') {
      baseRotateLeft();
      delay(response_time);
    }

    // Поворот базы манипулятора
    if (state == 'O') {
      baseRotateRight();
      delay(response_time);
    }


    // Плечо манипулятора опускается
    if (state == 'c') {

      shoulderServoForward();
      delay(response_time);

    }

    // Плечо манипулятора поднимается
    if (state == 'C') {

      shoulderServoBackward();
      delay(response_time);

    }

    // Плечо манипулятора опускается
    if (state == 'p') {

      elbowServoForward();
      delay(response_time);

    }

    // Плечо манипулятора поднимается
    if (state == 'P') {

      elbowServoBackward();
      delay(response_time);

    }


    // Запястье манипулятора 1 поднимается
    if (state == 'U') {

      wristServo1Backward();
      delay(response_time);
    }

    // Запястье манипулятора 1 опускается
    if (state == 'G') {

      wristServo1Forward();
      delay(response_time);

    }


    // Запястье манипулятора 2 вращается по часовой стрелке
    if (state == 'R') {

      wristServoCW();
      delay(response_time);

    }

    // Запястье манипулятора 2 вращается против часовой стрелке
    if (state == 'L') {

      wristServoCCW();
      delay(response_time);

    }


    // Открыть клешню манипулятора
    if (state == 'F') {
      gripperServoBackward();
      delay(response_time);

    }

    // Заклыть клешню манипулятора
    if (state == 'f') {
      gripperServoForward();
      delay(response_time);
    }


  }
}

// Эти функции перемещают серводвигатели в определенном направлении в течение определенного времени.

void gripperServoForward() {

  if (servo_joint_4_parking_pos_i > servo_joint_4_min_pos) {
    HCPCA9685.Servo(5, servo_joint_4_parking_pos_i);
    delay(response_time); // Время задержки, необходимое для поворота сервопривода, на заданное приращение
    Serial.println(servo_joint_4_parking_pos_i);
    servo_joint_4_parking_pos_i = servo_joint_4_parking_pos_i - servo_joint_4_pos_increment;

  }
}

void gripperServoBackward() {

  if (servo_joint_4_parking_pos_i < servo_joint_4_max_pos) {
    HCPCA9685.Servo(5, servo_joint_4_parking_pos_i);
    delay(response_time);
    Serial.println(servo_joint_4_parking_pos_i);
    servo_joint_4_parking_pos_i = servo_joint_4_parking_pos_i + servo_joint_4_pos_increment;

  }

}

void wristServoCW() {

  if (servo_joint_3_parking_pos_i > servo_joint_3_min_pos) {
    HCPCA9685.Servo(4, servo_joint_3_parking_pos_i);
    delay(response_time_4);
    Serial.println(servo_joint_3_parking_pos_i);
    servo_joint_3_parking_pos_i = servo_joint_3_parking_pos_i - servo_joint_3_pos_increment;

  }

}

void wristServoCCW() {

  if (servo_joint_3_parking_pos_i < servo_joint_3_max_pos) {
    HCPCA9685.Servo(4, servo_joint_3_parking_pos_i);
    delay(response_time_4);
    Serial.println(servo_joint_3_parking_pos_i);
    servo_joint_3_parking_pos_i = servo_joint_3_parking_pos_i + servo_joint_3_pos_increment;

  }

}

void wristServo1Forward() {

  if (servo_joint_2_parking_pos_i < servo_joint_2_max_pos) {
    HCPCA9685.Servo(3, servo_joint_2_parking_pos_i);
    delay(response_time);
    Serial.println(servo_joint_2_parking_pos_i);

    servo_joint_2_parking_pos_i = servo_joint_2_parking_pos_i + servo_joint_2_pos_increment;

  }


}

void wristServo1Backward() {

  if (servo_joint_2_parking_pos_i > servo_joint_2_min_pos) {
    HCPCA9685.Servo(3, servo_joint_2_parking_pos_i);
    delay(response_time);
    Serial.println(servo_joint_2_parking_pos_i);

    servo_joint_2_parking_pos_i = servo_joint_2_parking_pos_i - servo_joint_2_pos_increment;

  }

}


void elbowServoForward() {

  if (servo_joint_L_parking_pos_i < servo_joint_L_max_pos) {
    HCPCA9685.Servo(0, servo_joint_L_parking_pos_i);
    HCPCA9685.Servo(1, (servo_joint_L_max_pos - servo_joint_L_parking_pos_i));

    delay(response_time);
    Serial.println(servo_joint_L_parking_pos_i);

    servo_joint_L_parking_pos_i = servo_joint_L_parking_pos_i + servo_joint_L_pos_increment;
    servo_joint_R_parking_pos_i = servo_joint_L_max_pos - servo_joint_L_parking_pos_i;

  }
}

void elbowServoBackward() {
  if (servo_joint_L_parking_pos_i > servo_joint_L_min_pos) {
    HCPCA9685.Servo(0, servo_joint_L_parking_pos_i);
    HCPCA9685.Servo(1, (servo_joint_L_max_pos - servo_joint_L_parking_pos_i));

    delay(response_time);
    Serial.println(servo_joint_L_parking_pos_i);


    servo_joint_L_parking_pos_i = servo_joint_L_parking_pos_i - servo_joint_L_pos_increment;
    servo_joint_R_parking_pos_i = servo_joint_L_max_pos - servo_joint_L_parking_pos_i;

  }

}

void shoulderServoForward() {

  if (servo_joint_1_parking_pos_i < servo_joint_1_max_pos) {
    HCPCA9685.Servo(2, servo_joint_1_parking_pos_i);
    delay(response_time);
    Serial.println(servo_joint_1_parking_pos_i);

    servo_joint_1_parking_pos_i = servo_joint_1_parking_pos_i + servo_joint_1_pos_increment;

  }

}

void shoulderServoBackward() {


  if (servo_joint_1_parking_pos_i > servo_joint_1_min_pos) {
    HCPCA9685.Servo(2, servo_joint_1_parking_pos_i);
    delay(response_time);
    Serial.println(servo_joint_1_parking_pos_i);

    servo_joint_1_parking_pos_i = servo_joint_1_parking_pos_i - servo_joint_1_pos_increment;

  }
}

void baseRotateLeft() {
  // По часовой стрелке
  digitalWrite(dirPin, HIGH);
  // Вращение шагового мотора
  for (int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay);
  }
  delay(response_time);
}


void baseRotateRight() {

  // Против часовой стрелки
  digitalWrite(dirPin, LOW);
  // Вращение шагового мотора
  for (int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay);
  }
  delay(response_time);
}
