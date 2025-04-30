#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

String command;   
// String to store app command state.
int speed = 128;         
// Speed of the motors. 0-255
int speed_Coeff = 3;

// 定义 L9110S 电机驱动模块的输入引脚
// 引脚定义
// 这里使用了 NodeMCU 开发板的 D1-D8 引脚作为电机驱动模块的输入引脚
// MotorA 是右前方的电机，MotorB 是右后方的电机，MotorC 是左前方的电机，MotorD 是左后方的电机
// MotorA1 和 MotorA2 控制 MotorA 的正转和反转，MotorB1 和 MotorB2 控制 MotorB 的正转和反转，依此类推
const int MotorA1 = D1;   
const int MotorA2 = D2;
const int MotorB1 = D3;
const int MotorB2 = D4;
const int MotorC1 = D5;
const int MotorC2 = D6;
const int MotorD1 = D7;
const int MotorD2 = D8;

// 定义 WiFi 信息
const char* ssid = "NodeMCU Car";
const char* password = "12345678"; // 设置 WiFi 密码

ESP8266WebServer server(80);

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
  if (!WiFi.softAP(ssid, password)) {
    Serial.println("Failed to start WiFi AP");
    while (1); // 若启动失败，进入死循环
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Starting WEB-server 
  server.on("/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot);
  server.begin();

}

void goforwards() { 
    analogWrite(MotorA1, speed);
    analogWrite(MotorA2, 0);
    analogWrite(MotorB1, speed);
    analogWrite(MotorB2, 0);
    analogWrite(MotorC1, speed);
    analogWrite(MotorC2, 0);
    analogWrite(MotorD1, speed);
    analogWrite(MotorD2, 0);
}

void goBack() { 
    analogWrite(MotorA1, 0);
    analogWrite(MotorA2, speed);
    analogWrite(MotorB1, 0);
    analogWrite(MotorB2, speed);
    analogWrite(MotorC1, 0);
    analogWrite(MotorC2, speed);
    analogWrite(MotorD1, 0);
    analogWrite(MotorD2, speed);
}

// 右侧的两个电机向前，左侧的两个电机向后
void goLeft() {   
    analogWrite(MotorA1, speed);
    analogWrite(MotorA2, 0);
    analogWrite(MotorB1, speed);
    analogWrite(MotorB2, 0);
    analogWrite(MotorC1, 0);
    analogWrite(MotorC2, speed);
    analogWrite(MotorD1, 0);
    analogWrite(MotorD2, speed);
}

// 右侧的两个电机向后，左侧的两个电机向前
void goRight() { 
    analogWrite(MotorA1, 0);
    analogWrite(MotorA2, speed);
    analogWrite(MotorB1, 0);
    analogWrite(MotorB2, speed);
    analogWrite(MotorC1, speed);
    analogWrite(MotorC2, 0);
    analogWrite(MotorD1, speed);
    analogWrite(MotorD2, 0);
}

// 左侧的电机较快，右侧的电机较慢
void goAheadRight() { 
    analogWrite(MotorA1, speed / speed_Coeff);
    analogWrite(MotorA2, 0);
    analogWrite(MotorB1, speed / speed_Coeff);
    analogWrite(MotorB2, 0);
    analogWrite(MotorC1, speed);
    analogWrite(MotorC2, 0);
    analogWrite(MotorD1, speed);
    analogWrite(MotorD2, 0);
}

// 右侧的电机较快，左侧的电机较慢
void goAheadLeft() { 
    analogWrite(MotorA1, speed);
    analogWrite(MotorA2, 0);
    analogWrite(MotorB1, speed);
    analogWrite(MotorB2, 0);
    analogWrite(MotorC1, speed / speed_Coeff);
    analogWrite(MotorC2, 0);
    analogWrite(MotorD1, speed / speed_Coeff);
    analogWrite(MotorD2, 0);
}

// 左侧的电机较快，右侧的电机较慢 但是是倒车
void goBackRight() { 
    analogWrite(MotorA1, 0);
    analogWrite(MotorA2, speed / speed_Coeff);
    analogWrite(MotorB1, 0);
    analogWrite(MotorB2, speed / speed_Coeff);
    analogWrite(MotorC1, speed);
    analogWrite(MotorC2, 0);
    analogWrite(MotorD1, speed);
    analogWrite(MotorD2, 0);
}

// 右侧的电机较快，左侧的电机较慢 但是是倒车
void goBackLeft() { 
    analogWrite(MotorA1, speed);
    analogWrite(MotorA2, 0);
    analogWrite(MotorB1, speed);
    analogWrite(MotorB2, 0);
    analogWrite(MotorC1, 0);
    analogWrite(MotorC2, speed / speed_Coeff);
    analogWrite(MotorD1, 0);
    analogWrite(MotorD2, speed / speed_Coeff);
}

void stop() { 
    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, LOW);
    digitalWrite(MotorC1, LOW);
    digitalWrite(MotorC2, LOW);
    digitalWrite(MotorD1, LOW);
    digitalWrite(MotorD2, LOW);
}

// 处理 HTTP 请求
// 通过按钮控制电机的运动
// 通过滑块控制电机的速度
// 当用户点击网页上的按钮（如 “Forward”“Backward” 等）时，JavaScript 函数sendCommand会被调用。
// 这个函数使用XMLHttpRequest对象创建一个 GET 请求，并将相应的指令（如 “F”“B” 等）发送到 ESP8266 的 IP 地址。
// ESP8266 接收到这些指令后，会执行相应的函数来控制小车的运动。
void HTTP_handleRoot() {
    String html = "<html><body>";
    html += "<h1>ESP8266 Car Control</h1>";
    html += "<button onclick=\"sendCommand('F')\">Forward</button><br>";  // 定义按钮
    html += "<button onclick=\"sendCommand('B')\">Backward</button><br>";
    html += "<button onclick=\"sendCommand('L')\">Left</button><br>";
    html += "<button onclick=\"sendCommand('R')\">Right</button><br>";
    html += "<button onclick=\"sendCommand('I')\">Ahead Right</button><br>";
    html += "<button onclick=\"sendCommand('G')\">Ahead Left</button><br>";
    html += "<button onclick=\"sendCommand('J')\">Back Right</button><br>";
    html += "<button onclick=\"sendCommand('H')\">Back Left</button><br>";
    html += "<button onclick=\"sendCommand('S')\">Stop</button><br>";
    html += "<label for=\"speedSlider\">Speed:</label>";      // 定义滑块
    html += "<input type=\"range\" id=\"speedSlider\" min=\"0\" max=\"255\" value=\"" + String(speed) + "\" oninput=\"updateSpeed(this.value)\"><br>";
    html += "<script>";
    html += "function sendCommand(command) {";
    html += "    var xhr = new XMLHttpRequest();";
    html += "    xhr.open('GET', '/?State=' + command, true);";
    html += "    xhr.send();";
    html += "}";
    html += "function updateSpeed(value) {";
    html += "    var xhr = new XMLHttpRequest();";
    html += "    xhr.open('GET', '/?State=' + value, true);";
    html += "    xhr.send();";
    html += "}";
    html += "</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}


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
    else if (command == "0") speed = 0;
    else if (command == "1") speed = 25;
    else if (command == "2") speed = 50;
    else if (command == "3") speed = 75;
    else if (command == "4") speed = 100;
    else if (command == "5") speed = 125;
    else if (command == "6") speed = 150;
    else if (command == "7") speed = 175;
    else if (command == "8") speed = 200;
    else if (command == "9") speed = 240;
    else if (command == "S") stop();
}    