#include <Sim800L.h>
#include <SoftwareSerial.h>

const int relay1Pin = 30;
const int relay2Pin = 31;
const int waterLevelSensorPin = A0;
Sim800L gsm(10, 11); // RX, TX

bool powerStatus = LOW;
int waterLevel = 0;

void setup() {
  Serial.begin(9600);
  gsm.begin(4800);   
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
}

void loop() {
  readWaterLevel();
  readSerial();
}

void switchOn() {
  powerStatus = HIGH;
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
}

void switchOff() {
  powerStatus = LOW;
  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);
}

void readWaterLevel() {
  waterLevel = analogRead(waterLevelSensorPin);
}

void sendSMS() {
  char* number = "*********";
  char* text = "YOUR SOCKET HAS BEEN EXPOSED TO WATER!!!";
  gsm.sendSms(number,text);
  delay(5000);
}

void readSerial() {
  if (Serial.available()) {
    char data = Serial.read();
    Serial.println(data);
    if (data == 'q') {
      Serial.println("{ 'powerStatus': " + String(powerStatus) + ", 'waterLevel': " + String(waterLevel) + " }");
    } else if (data == 'w') {
      switchOn();
    } else if (data == 'e') {
      switchOff();
    } else if (data == 'r') {
      sendSMS();
    }
  }
}
