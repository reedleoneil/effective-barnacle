#include <Sim800L.h>
#include <SoftwareSerial.h>

const int relay1Pin = 30;
const int relay2Pin = 31;
const int waterLevelSensorPin = A0;
Sim800L gsm(7, 8); // RX, TX

bool powerStatus = LOW;
int waterLevel = 0;
bool sms = false;

void setup() {
  Serial.begin(9600);
  gsm.begin(4800);   
  gsm.setSleepMode(false);
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
  if (waterLevel > 350 && sms == false) {
    sms = true;
    SendMessage();
  } else if (waterLevel > 350 && sms == true) {
    // do nothing
  } else {
    sms = false;
  }
  delay(100);
}

void sendSMS() {
  char* number = "";
  char* text = "Warning: Water has reached critical level!";
  gsm.sendSms(number,text);
}

void SendMessage()
{
  gsm.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  gsm.println("AT+CMGS=\"numberhere\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = "Warning: Water has reached critical level!";
  gsm.println(SMS);
  delay(100);
  gsm.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void readSerial() {
  if (Serial.available()) {
    char data = Serial.read();
    Serial.println(data);
    if (data == 'q') {
      Serial.println("{ \"powerStatus\": " + String(powerStatus) + ", \"waterLevel\": " + String(waterLevel) + " }");
    } else if (data == 'w') {
      switchOn();
    } else if (data == 'e') {
      switchOff();
    } else if (data == 'r') {
      sendSMS();
    }
  }
}
