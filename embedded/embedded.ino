#include <Sim800L.h>
#include <SoftwareSerial.h>

const int relay1Pin = 30;
const int relay2Pin = 31;
const int waterLevelSensorPin = A0;
Sim800L gsm(10, 11); // RX, TX

bool powerStatus = LOW;
int waterLevel = 0;
bool critical = false;
bool hasWater = false;
int acknowledge = false;

String phone_number = "09161195643"; //09161195643

String message_1 = "Warning! The water level is close to critical level.";
String message_2 = "Attention! The water level has reached the critical level. Please turn off the socket for safety assurance, else, wait for the system to work.";
String message_3 = "Attention! The water level went down from the critical level.";
String message_4 = "Attention! The water subsided and is now out of critical level.";

void setup() {
  Serial.begin(9600);
  gsm.begin(4800);   
  gsm.setSleepMode(false);
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  delay(5000);
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
}

void loop() {
  readWaterLevel();
  readSerial();
  readSMS();
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
  //Serial.println(waterLevel);

  if (waterLevel <= 10) {
    if (hasWater) {
      hasWater = false;
      SendMessage(phone_number, message_4); //Serial.println(message_4);
    }
  } else if (waterLevel > 10 && waterLevel <= 350) {
    if (critical) {
      critical = false;
      SendMessage(phone_number, message_3); //Serial.println(message_3);
    }
    if (!hasWater) {
      hasWater = true;
      SendMessage(phone_number, message_1); //Serial.println(message_1);
    }
  } else if (waterLevel > 350) {
    if (!critical) {
      critical = true;
      SendMessage(phone_number, message_2); //Serial.println(message_2);
    }
  }
  
  delay(100);
}

void sendSMS() {
  char* number = "09161195643";
  char* text = "Warning: Water has reached critical level!";
  gsm.sendSms(number,text);
}

void SendMessage(String number, String message)
{
  gsm.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  gsm.println("AT+CMGS=\"" + phone_number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = message;
  gsm.println(SMS);
  delay(100);
  gsm.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void readSMS() { 
  String textMessage;
  if(gsm.available()>0){
    textMessage = gsm.readString();
    if (textMessage.indexOf("ON")>=0) {
      switchOn();  
    } else if (textMessage.indexOf("OFF")>=0) {
      switchOff();  
    }
  } 
}

void readSerial() {
  if (Serial.available()) {
    char data = Serial.read();
    Serial.println(data);
    if (data == 'q') {
      Serial.println("{ \"powerStatus\": " + String(powerStatus) + ", \"waterLevel\": " + String(waterLevel) + ", \"acknowledge\": " + String(acknowledge) + " }");
      acknowledge = false;
    } else if (data == 'w') {
      acknowledge = true;
      switchOn();
    } else if (data == 'e') {
      acknowledge = true;
      switchOff();
    } else if (data == 'r') {
      sendSMS();
    } else if (data == 'a') {
      SendMessage(phone_number, message_1);
    } else if (data == 's') {
      SendMessage(phone_number, message_2);
    } else if (data == 'd') {
      SendMessage(phone_number, message_3);
    } else if (data == 'f') {
      SendMessage(phone_number, message_4);
    }
  }
}
