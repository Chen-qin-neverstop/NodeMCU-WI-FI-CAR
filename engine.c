#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

// 定义 L9110S 电机驱动模块的输入引脚
const int MotorA1 = D1;
const int MotorA2 = D2;
const int MotorB1 = D3;
const int MotorB2 = D4;
const int MotorC1 = D5;
const int MotorC2 = D6;
const int MotorD1 = D7;
const int MotorD2 = D8;

void setup() {
  // 设置引脚为输出模式
  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  pinMode(MotorC1, OUTPUT);
  pinMode(MotorC2, OUTPUT);
  pinMode(MotorD1, OUTPUT);
  pinMode(MotorD2, OUTPUT);

  Serial.begin(115200);

  // Connecting WiFi

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}

void goforwards(){ 

    analogWrite(MotorA1,speed);
    analogWrite(MotorA2,speed);
    analogWrite(MotorB1,speed);
    analogWrite(MotorB2,speed);
    analogWrite(MotorC1,speed);
    analogWrite(MotorC2,speed);
    analogWrite(MotorD1,speed);
    analogWrite(MotorD2,speed);

}

void stop(){ 

    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, LOW);
    digitalWrite(MotorC2, LOW);
    digitalWrite(MotorD1, LOW);
    digitalWrite(MotorD2, LOW);

}

String command;             //String to store app command state.
int speed = 800;         // 400 - 1023.
int speed_Coeff = 3;

const char* ssid = "NodeMCU Car";
ESP8266WebServer server(80);

void loop() {
    server.handleClient();
    
      command = server.arg("State");
      if (command == "F") goforwards();
      else if (command == "B") goBack();
      else if (command == "L") goLeft();
      else if (command == "R") goRight();
      else if (command == "I") goAheadRight();
      else if (command == "G") goAheadLeft();
      else if (command == "J") goBackRight();
      else if (command == "H") goBackLeft();
      else if (command == "0") speed = 400;
      else if (command == "1") speed = 470;
      else if (command == "2") speed = 540;
      else if (command == "3") speed = 610;
      else if (command == "4") speed = 680;
      else if (command == "5") speed = 750;
      else if (command == "6") speed = 820;
      else if (command == "7") speed = 890;
      else if (command == "8") speed = 960;
      else if (command == "9") speed = 1023;
      else if (command == "S") stop();
}