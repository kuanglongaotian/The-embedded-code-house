#include "Web_Service.h"

const char *host = "esp32";

uint16_t Count;
uint8_t LEDPin = 2, Cycles;

const char *username = "admin";     //用户名
const char *userpassword = "admin"; //用户密码

WebServer server(80);
boolean IsStartUpdate = false;

void LEDflash(uint8_t LEDflashTick)
{
  Cycles++;
  if (Cycles > LEDflashTick) digitalWrite(LEDPin, HIGH);
  else digitalWrite(LEDPin, LOW);
  if (Cycles == LEDflashTick * 2) Cycles = 0;
}
// 获取文件类型
String getContentType(String filename)
{
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void HandleFileFormSPIFFS(String webAddress) 
{              
  if (SPIFFS.exists(webAddress)) 
  {                                                 // 如果访问的文件可以在SPIFFS中找到
    File file = SPIFFS.open(webAddress, "r");
    String contentType = getContentType(webAddress);// 获取文件类型     
    server.streamFile(file, contentType);           // 并且将该文件返回给浏览器
    file.close();                                                          
  }
  else// 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
  {server.send(404, "text/plain", "404 Not Found");}
}

void iniWebServer()
{
  if(SPIFFS.begin())// 启动闪存文件系统
  {Serial.println("SPIFFS started");} 
  else 
  {Serial.println("SPIFFS failed to start");}

  while(!MDNS.begin(host))
  {
    Serial.println("Error setting up MDNS responder!");
    delay(1000);
  }
  Serial.println("mDNS responder started");

  server.onNotFound//用Notfound取巧来访问所有html代码中的文件链接。打开网址和打开文件是不一样的
  (                //可以直接打开网址获取内容，也可以打开网址，再打开需要的文件。和html
    []()
    {
      String webAddress = server.uri();    // 获取用户请求网址信息
      if(webAddress=="/")                 // 如果访问到ip地址的根目录
      {
        if(!server.authenticate(username, userpassword)) //校验用户是否登录
        {
          return server.requestAuthentication();          //请求进行用户登录认证
        }
        webAddress = "/web_interface.html";
        HandleFileFormSPIFFS(webAddress);
        //server.send(200, "text/plain", "登录成功!"); //登录成功则显示真正的内容
      }
      else HandleFileFormSPIFFS(webAddress); 
    }
  );

  server.on
  ("/web_servce", HTTP_GET, 
    []()
    {
      server.sendHeader("Connection", "close");
      HandleFileFormSPIFFS("/web_servce.html");
      Serial.println("You can upload file..."); 
    }
  );
  
  server.on
  ("/update", HTTP_POST, 
    []()
    {
      server.sendHeader("Connection", "close");
      Serial.print("Start upload...");
      String Result;
      Result = (Update.hasError()) ? "FAIL" : "OK";
      Serial.println(Result);
      server.send(200, "text/plain", Result);
      ESP.restart(); 
    },
    []()
    {
      HTTPUpload &upload = server.upload();
      Serial.print(".");
      if (upload.status == UPLOAD_FILE_START)
      {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        IsStartUpdate = true;
        if (!Update.begin(UPDATE_SIZE_UNKNOWN))
        { // start with max available size
          Update.printError(Serial);
        }
      }
      else if (upload.status == UPLOAD_FILE_WRITE)
      {
        /* flashing firmware to ESP*/
        Serial.print("-");
        Count++;
        if ((Count % 20) == 0)
        {
          Serial.println("");
        }
        LEDflash(2);
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
        {
          Update.printError(Serial);
          Serial.print("Updated Fail.");
        }
      }
      else if (upload.status == UPLOAD_FILE_END)
      {
        if (Update.end(true))
        { // true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        }
        else
        {
          Update.printError(Serial);
        }
      }
    }
  );

  server.begin();
}

