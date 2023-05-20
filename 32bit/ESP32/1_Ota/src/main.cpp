#include <WiFiClient.h>
#include <HTTPClient.h>
#include "Wifi_Service.h"
#include "Web_Service.h"

// #define URL "http://www.example.com"
// const char* host2 = "www.example.com"; // 网络服务器地址

long Time1;

String Version = "The Current Version: 1.1.2";

void Web_Update_Init(void)
{
  WiFi.onEvent(MyWiFiEvent);
  Wifi_Init();
  iniWebServer();
  Serial.println(Version);
}

void setup(void)
{
  delay(5000);
  Serial.begin(115200);
  Web_Update_Init();
  // 建立WiFi客户端对象，对象名称client
  WiFiClient client;   
  client.find("c:"); 
  // // 建立字符串，用于HTTP请求
  // String httpRequest =  String("GET /") + " HTTP/1.1\r\n" +
  //                       "Host: " + host2 + "\r\n" +
  //                       "Connection: close\r\n" +
  //                       "\r\n"; 
  // // 通过串口输出连接服务器名称以便查阅连接服务器的网址                      
  // Serial.print("Connecting to "); 
  // Serial.print(host2); 

  // if (client.connect(host2, 80))
  // { 
  //   Serial.println(" Success!");        // 连接成功后串口输出“Success”信息
  //   client.print(httpRequest);          // 向服务器发送HTTP请求
  //   Serial.println("Sending request: ");// 通过串口输出HTTP请求信息内容以便查阅
  //   Serial.println(httpRequest);     
    
  //   Serial.println("Web Server Response:");        
  //   while (client.connected() || client.available())
  //   { 
  //     if (client.available())
  //     {
  //       String line = client.readStringUntil('\n');
  //       Serial.println(line);
  //     }
  //   } 
  //   client.stop();                      // 断开与服务器的连接
  //   Serial.print("Disconnected from "); // 并且通过串口输出断开连接信息
  //   Serial.print(host2);
  // }
  // else
  //  {    // 如果连接不成功则通过串口输出“连接失败”信息
  //   Serial.println(" connection failed!");
  //   client.stop();
  //  } 
}

void loop(void)
{
  server.handleClient();

  // if (millis() - Time1 > 2000 && !IsStartUpdate)
  // {
  //   Time1 = millis();
  //   Serial.print("Please connect: ");
  //   Serial.print(myIP);
  //   Serial.println(" by explore");
  // }

  //   File file = SPIFFS.open("/README.txt", "r");
  //   if (!file)
  //   {
  //     Serial.println("Failed to open file for reading");
  //     return;
  //   };
  //   while(file.available())
  //   {
  //     Serial.write(file.read());
  //   }
  //   file.close();
}
