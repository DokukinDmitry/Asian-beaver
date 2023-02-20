// #include <SoftwareSerial.h>
// int data;
// SoftwareSerial bt(0,1);
// void setup() {
//   // put your setup code here, to run once:
//   pinMode(13,OUTPUT);
//   Serial.begin(9600);
//   bt.begin(9600);

// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   if (bt.available()){
//     bt.listen();
//     data=bt.readString();
//     Serial.println(data);
//   //   if (data==1) digitalWrite(13,1);
//   //   if (data==2) digitalWrite(13,0);
//     delay(100);
//   }
//   // Serial.write("1");
//   // bt.write("1");
//   // delay(1000);
// }

// // void setup()  {
  
// // }

// // void loop() {
 
// // }


#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(0, 1); // RX, TX
 
void setup()  
{
  // Инициализируем последовательный интерфейс и ждём открытия порта:
  Serial.begin(9600);
  // Serial.write("Fake moon!");
  // устанавливаем скорость передачи данных для последовательного порта
  mySerial.begin(9600);
  // mySerial.println("Hello, world?");
}
 
void loop()
{
  if (mySerial.available())
    Serial.println(mySerial.read());
  // if (Serial.available())
  //   mySerial.write(Serial.read());
}