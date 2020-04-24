#include <Servo.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <FS.h>
ESP8266WebServer server(80);
MDNSResponder mdns;
IPAddress ip;
String temp;

const char *ssid     = "cjh";
const char *password = "13867521";

Servo servoX;
Servo servoY;
int posX = 100; //X轴电机初始位置
int posY = 100; //Y轴电机初始位置
void randomSeed(unsigned int seed = 11451);

void setup() {
  servoX.attach(0); //X轴电机接D3
  servoX.write(posX);
  servoY.attach(2); //Y轴电机接D4
  servoY.write(posY);
  pinMode(14, OUTPUT); //投食电机接D5
  pinMode(12, OUTPUT); //弹射电机接D6
  delay(3000);

  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Starting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Done.");
  ip = WiFi.localIP(); //读取被分配的IP地址
  int buf[4];
  for (int i = 0; i < 4; i++)
  {
    buf[i] = ip[i];
  }
  temp = String(buf[0]);
  temp += '.';
  temp += String(buf[1]);
  temp += '.';
  temp += String(buf[2]);
  temp += '.';
  temp += String(buf[3]);
  Serial.print("IP: ");
  Serial.println(temp);

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.on("/RandomMove", []() {
    sendHtml();
    RandomMove();
  });

  server.on("/Drop", []() {
    sendHtml();
    Drop();
  });

  server.on("/Biu", []() {
    sendHtml();
    Biu();
  });

  server.begin();
};

void loop() {
  server.handleClient();
};

//————————————————

void RandomMove() {
  posX = random(90, 130);
  servoX.write(posX);
  posY = random(90, 130);
  servoY.write(posY);
};

void Drop() {
  digitalWrite(14, HIGH);
  delay(10000);
  digitalWrite(14, LOW);
};

void Biu() {
  digitalWrite(12, HIGH);
  delay(10000);
  digitalWrite(12, LOW);
};

void handleRoot() {
  sendHtml();
}

void sendHtml() {
  server.send(200, "text/html", "<!DOCTYPE html><html lang='zh-cn'><head><meta name='renderer' content='webkit'/><meta name='force-rendering' content='webkit'/><meta http-equiv='X-UA-Compatible' content='IE=Edge,chrome=1'/><link rel='icon' href='data:image/ico;base64,aWNv'><meta http-equiv='Content-Type' content='text/html; charset=utf-8'><title>喂猫机控制主页</title><style>body{background-color:#ECE4D9;}.k{width:100%;border:none;padding:15px 32px;text-align:center;text-decoration:none;display:inline-block;font-size:100px;border-radius:20px;margin-bottom:20px;}.c{text-align:center;}.b{font-size:100px;}.c1{background-color:#ffb6b9;}.c2{background-color:#bbded6;}.c3{background-color:#8ac6d1;}</style></head><body><div style='border-radius: 25px; border: 5px solid #555; padding: 20px; width: auto; height: auto;'><iframe src='//www.17sucai.com/preview/1424582/2019-12-03/m/demo.html' frameborder='0' width='100%' height='100%' style='z-index:-1;position:absolute;'></iframe><h1 class='c b'>喂猫机控制</h1><h3 class='c'>说明：逗猫：单次激光移动；投食：下落电机工作10秒；Biu：弹射电机工作10秒。</h3><h1><a href='/RandomMove'><button class='k c1'>逗猫</button></a><a href='/Drop'><button class='k c2'>投食</button></a><a href='/Biu'><button class='k c3'>Biu~</button></a></h1></div></body></html>");
}

void handleNotFound() {
  server.send(404, "text/html", "<!DOCTYPE HTML><html lang='zh-cn'><head><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge,chrome=1'><meta name='viewport' content='width=device-width, initial-scale=1'><title>404</title></head><body>404 not found<a href='/pdf.gz'><button class='k c2'>简历</button></a></body></html>");
}
