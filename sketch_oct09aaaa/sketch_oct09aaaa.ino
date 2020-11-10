#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

#include <Servo.h>

int dataPin  = 10;   //Пин подключен к DS входу 74HC595
int latchPin = 11;  //Пин подключен к ST_CP входу 74HC595
int clockPin = 12;  //Пин подключен к SH_CP входу 74HC595

Servo myservo;

double dst;
int angle = 30;
bool obst = false;
int stopAngle = 0;

void move(int dir) {
   if (dir == 1) {  //вперед
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    digitalWrite(6,HIGH);
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
    digitalWrite(9,HIGH);
   }
    if (dir == 2) { //назад
    digitalWrite(3,LOW);
    digitalWrite(2,HIGH);
    digitalWrite(5,LOW);
    digitalWrite(4,HIGH);
    digitalWrite(7,HIGH);
    digitalWrite(6,LOW);
    digitalWrite(9,LOW);
    digitalWrite(8,HIGH);
   }
    if (dir == 3) {//направо
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    digitalWrite(6,LOW);
    digitalWrite(7,HIGH);
    digitalWrite(8,HIGH);
    digitalWrite(9,LOW);
   }
   if (dir == 4) {//налево
    digitalWrite(3,LOW);
    digitalWrite(2,HIGH);
    digitalWrite(5,LOW);
    digitalWrite(4,HIGH);
    digitalWrite(7,LOW);
    digitalWrite(6,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(8,LOW);
   }
   if (dir == 5) {//стоп
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
   }
  
}

void setup(){
     myservo.attach(13);
     myservo.write(angle);
    //Serial.begin(9600);
    lox.begin();
    
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
    
   
}

void loop(){
  VL53L0X_RangingMeasurementData_t measure;

if (!obst) {
  for (int i=30; i<=150; i=i+5){ 
    myservo.write(i);
    lox.rangingTest(&measure, false);
    dst = measure.RangeMilliMeter/10;
    //Serial.println(dst);
    move(1);
    //Serial.println("вперед");

  digitalWrite(latchPin, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
  shiftOut(dataPin, clockPin, LSBFIRST, B00010000);   // передаем последовательно на dataPin
  digitalWrite(latchPin, HIGH);                       //"защелкиваем" регистр, тем самым устанавливая значения на выходах
   
    if (dst>=0 && dst<=40) {
      move(5);
      //Serial.println("стоп");

  digitalWrite(latchPin, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
  shiftOut(dataPin, clockPin, LSBFIRST, B10000000);   // передаем последовательно на dataPin
  digitalWrite(latchPin, HIGH);                       //"защелкиваем" регистр, тем самым устанавливая значения на выходах
      delay(100);
      obst = true;
      stopAngle = i;
      break;
    }
  }
}

if (!obst) {
  for (int i=150; i>=30; i=i-5){ 
    myservo.write(i);
    lox.rangingTest(&measure, false);
    dst = measure.RangeMilliMeter/10; 
    //Serial.println(dst);
    move(1);
    //Serial.println("вперед");

  digitalWrite(latchPin, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
  shiftOut(dataPin, clockPin, LSBFIRST, B00010000);   // передаем последовательно на dataPin
  digitalWrite(latchPin, HIGH);                       //"защелкиваем" регистр, тем самым устанавливая значения на выходах
   
    if (dst >=0 && dst <=40) {
      move(5);
      //Serial.println("стоп");
    delay(100);
    digitalWrite(latchPin, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
    shiftOut(dataPin, clockPin, LSBFIRST, B10000000);   // передаем последовательно на dataPin
    digitalWrite(latchPin, HIGH);                       //"защелкиваем" регистр, тем самым устанавливая значения на выходах
   
      obst = true;
      stopAngle = i;
      break;
    }
   
    //delay(150);
  }
}
//Serial.print("angle");
//Serial.println(stopAngle);

if (obst) {
  if (stopAngle >= 91) {
    move(3);
    //Serial.println("направо");

  digitalWrite(latchPin, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
  shiftOut(dataPin, clockPin, LSBFIRST, B00100000);   // передаем последовательно на dataPin
  digitalWrite(latchPin, HIGH);                       //"защелкиваем" регистр, тем самым устанавливая значения на выходах
   
    lox.rangingTest(&measure, false);
    dst = measure.RangeMilliMeter/10;
    if (dst>41 || dst <0) {
      obst = false; 
    }   
  }
    if (stopAngle <= 89) {
    move(4);
    //Serial.println("налево");

  digitalWrite(latchPin, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
  shiftOut(dataPin, clockPin, LSBFIRST, B01000000);   // передаем последовательно на dataPin
  digitalWrite(latchPin, HIGH);                       //"защелкиваем" регистр, тем самым устанавливая значения на выходах
  
    lox.rangingTest(&measure, false);
    dst = measure.RangeMilliMeter/10;
    if (dst>41 || dst <0) {
      obst = false; 
    }   
  }
    if (stopAngle == 90) {
    move(3);
    //Serial.println("направо");

    digitalWrite(latchPin, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
    shiftOut(dataPin, clockPin, LSBFIRST, B00100000);   // передаем последовательно на dataPin
    digitalWrite(latchPin, HIGH);                       //"защелкиваем" регистр, тем самым устанавливая значения на выходах
  
    lox.rangingTest(&measure, false);
    dst = measure.RangeMilliMeter/10;
    if (dst>41 || dst <0) {
      obst = false; 
    }   
  }
//Serial.println(obst);
}
}
