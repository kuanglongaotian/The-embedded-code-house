#include <WiFiClient.h>
//#include <cJSON.h>
#include "ArduinoJson.h"
#include "Wifi_Service.h"
#include "Web_Service.h"

long Time1;

String Version = "The Current Version: 1.1.2";

String Weather_key = "SpPxzHG-UtC-CD92A";  // 密钥
String reqLocation = "Nanjing";            // 城市
String reqUnit = "c";                      // 摄氏/华氏

WiFiClient client;

StaticJsonDocument<768> doc;

const char* Weather_host = "api.seniverse.com";     // 将要连接的服务器地址

void Web_Update_Init(void)
{
  WiFi.onEvent(MyWiFiEvent);
  Wifi_Init();
  iniWebServer();
  Serial.println(Version);
}

void Get_weather()
{
  String reqRes = "/v3/weather/now.json?key=" + Weather_key +
                  + "&location=" + reqLocation + 
                  "&language=en&unit=" +reqUnit;
  // 建立http请求信息
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
                              "Host: " + Weather_host + "\r\n" + 
                              "Connection: close\r\n\r\n";
  Serial.println(""); 
  Serial.print("Connecting to "); 
  Serial.print(Weather_host);
  // 尝试连接服务器
  if (client.connect(Weather_host, 80))
  {
    Serial.println(" Success!");
    // 向服务器发送http请求信息
    client.print(httpRequest);
    Serial.println("Sending request: ");
    Serial.println(httpRequest);  
    // 获取并显示服务器响应状态行 
    String status_response = client.readStringUntil('\n');
    Serial.print("status_response: ");
    Serial.println(status_response);
    // 使用find跳过HTTP响应头
    if (client.find("\r\n\r\n")) 
    {
      Serial.println("Found Header End. Start Parsing.");
    } 
    // 利用ArduinoJson库解析心知天气响应信息
    //parseInfo(client); 
    DeserializationError error = deserializeJson(doc, client);
    //deserializeJson(doc, client);
    if (error) 
    {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
    JsonObject results_0 = doc["results"][0];
    JsonObject results_0_now = results_0["now"];
    const char* results_0_now_text = results_0_now["text"]; // "Sunny"
    const char* results_0_now_code = results_0_now["code"]; // "0"
    const char* results_0_now_temperature = results_0_now["temperature"]; // "32"
    const char* results_0_last_update = results_0["last_update"]; // "2020-06-02T14:40:00+08:00" 
    // 通过串口监视器显示以上信息
    String results_0_now_text_str = results_0_now["text"].as<String>(); 
    int results_0_now_code_int = results_0_now["code"].as<int>(); 
    int results_0_now_temperature_int = results_0_now["temperature"].as<int>(); 
    String results_0_last_update_str = results_0["last_update"].as<String>();  

    Serial.println("======Weahter Now=======");
    Serial.printf("Weather Now: %s %d\n",results_0_now_text_str,results_0_now_code_int);
    Serial.printf("Temperature: %d\n",results_0_now_temperature_int);
    Serial.printf("Last Update: %s\n",results_0_last_update_str);
    Serial.println(F("========================"));
    } 
    else 
    {
      Serial.println(" connection failed!");
    }   
    //断开客户端与服务器连接工作
    client.stop(); 
}

void setup(void)
{
  delay(5000);
  Serial.begin(115200);
  Web_Update_Init();
  //Get_weather();
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
}
