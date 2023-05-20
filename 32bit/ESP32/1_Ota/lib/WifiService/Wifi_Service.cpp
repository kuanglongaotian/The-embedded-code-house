#include "Wifi_Service.h"

const char *ssid_1 = "HONOR V20";
const char *password_1 = "ad3ebeadacf8";
const char *ssidAP = "ESP32AP";
const char *APpassword = "12345678";

IPAddress myIP;
IPAddress local_IP(192,168,1,123); // 手动设置ESP32联网后的IP
IPAddress gateway(192,168,1,1);    // 设置网关IP（通常网关IP是WiFI路由IP）
IPAddress subnet(255,255,255,0);   // 设置子网掩码
IPAddress dns(192,168,1,1);        // 设置局域网DNS的IP（通常局域网DNS的IP是WiFI路由IP）
//boolean isDeviceConnected;

u8_t Wifi_Connect(const char *ssid, const char *password)
{
  u8_t Time = 0;

  Serial.printf("Start connect %s\n", ssid);
  WiFi.disconnect(false, true); //允许使用网络功能，清空配置
  WiFi.mode(WIFI_MODE_STA);
  WiFi.enableSTA(true);
  WiFi.enableAP(false);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
    Time++;
    if (Time == 10)
    {
      Serial.println("\nConnection failed!!");
      WiFi.disconnect(true, true);
      return 0;
    }
  }
  myIP = WiFi.localIP();
  Serial.println("\nConnection finshed");
  Serial.print("IP address: ");
  Serial.println(myIP);
  return 1;
}

u8_t Wifi_AP()
{
  // pinMode(LEDPin, OUTPUT);
  esp_wifi_restore();
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_MODE_AP);
  WiFi.enableSTA(false);
  WiFi.enableAP(true);
  WiFi.softAP(ssidAP, APpassword);
  // Serial.println(WiFi.getMode());
  myIP = WiFi.softAPIP();
  Serial.printf("Please connect %s network\r\n", ssidAP);
  while (WiFi.softAPgetStationNum() == 0) //检查是否已连接
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnection finshed");
  Serial.print("IP address: ");
  Serial.println(myIP);
  return 1;
  // WiFi.onEvent(MyWiFiEvent);
}

void Wifi_Init()
{
  u8_t Isok, key = 0x30;
  String ssid;
  String password;

  Isok = Wifi_Connect(ssid_1, password_1);
  while (!Isok)
  {
    Serial.println("Please Choose");
    Serial.println("--------------------------------------------------");
    Serial.printf("          Reconnect........................1\n");
    Serial.printf("          Turn to AP mode...........2\n");
    Serial.println("--------------------------------------------------");
    while (1)
    {
      if (Serial.available())
      {
        key = Serial.read();
        break;
      }
    }
    switch (key)
    {
    case 0x31:
      Serial.printf("Please input ssid:\n");
      while (1)
      {
        if (Serial.available())
        {
          ssid = Serial.readString();
          Serial.println(ssid);
          break;
        }
      }
      Serial.printf("Please input password:\n");
      while (1)
      {
        if (Serial.available())
        {
          password = Serial.readString();
          Serial.println(password);
          break;
        }
      }
      Isok = Wifi_Connect(ssid.c_str(), password.c_str()); // String类型转换
      break;
    case 0x32:
      Isok = Wifi_AP();
      break;
    default:
      key = 0x30;
      Serial.println("Please input right num");
    }
  }
}

void MyWiFiEvent(WiFiEvent_t event)
{
  Serial.printf("[WiFi-event:%d]----", event);
  switch (event)
  {
  case SYSTEM_EVENT_WIFI_READY://0
    Serial.println("ESP32 WiFi就绪");
    break;
  case SYSTEM_EVENT_SCAN_DONE://1
    Serial.println("ESP32 WiFi完成扫描AP");
    break;
  case SYSTEM_EVENT_STA_START://2
    Serial.println("ESP32 WiFi站点模式启动");
    break;
  case SYSTEM_EVENT_STA_STOP://3
    Serial.println("ESP32 WiFi站点模式停止");
    break;
  case SYSTEM_EVENT_STA_CONNECTED://4
    Serial.println("ESP32 WiFi已连接至AP设备");
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED://5
    Serial.println("ESP32 WiFi断开已连接AP设备");
    break;
  case SYSTEM_EVENT_STA_AUTHMODE_CHANGE://6
    Serial.println("ESP32 WiFi作为STA,连接的AP改变了");
    break;
  case SYSTEM_EVENT_STA_GOT_IP://7
    Serial.print("ESP32 WiFi被分配到的ip地址:");
    Serial.println(WiFi.localIP());
    break;
  case SYSTEM_EVENT_STA_LOST_IP://8
    Serial.println("ESP32 WiFi失去IP,IP被重置为0");
    break;
  case SYSTEM_EVENT_STA_BSS_RSSI_LOW://9
    Serial.println("ESP32 station connected BSS rssi goes below threshold");      
    break; 
  case SYSTEM_EVENT_STA_WPS_ER_SUCCESS://10                                 从这里开始向下5个进入回调到一个意义不明的模式。然而配置是AP模式。
    //Serial.println("ESP32 station wps: succeeds in enrollee mode");
    Serial.println("ESP32 WiFi AP模式启动");
    break;
  case SYSTEM_EVENT_STA_WPS_ER_FAILED://11
    //Serial.println("ESP32 station wps: fails in enrollee mode");
    Serial.println("ESP32 WiFi AP模式停止");
    break;  
  case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT://12
    //Serial.println("ESP32 station wps: timeout in enrollee mode");
    Serial.println("一个设备连接到了ESP32 WiFi");
    Serial.printf("当前设备数量：%d\n",WiFi.softAPgetStationNum());
    break;
  case SYSTEM_EVENT_STA_WPS_ER_PIN://13
    //Serial.println("ESP32 station wps: pin code in enrollee mode");
    Serial.println("一个设备断开和ESP32 WiFi连接");
    Serial.printf("当前设备数量：%d\n",WiFi.softAPgetStationNum());
    break;
  case SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP://14
    //Serial.println("ESP32 station wps: overlap in enrollee mode");
    Serial.println("ESP32 WiFi为一个已连接设备分配了个IP");
    break;
  case SYSTEM_EVENT_AP_START://15
    Serial.println("ESP32 WiFi AP模式启动");
    break;
  case SYSTEM_EVENT_AP_STOP://16
    Serial.println("ESP32 WiFi AP模式停止");
    break;
  case SYSTEM_EVENT_AP_STACONNECTED://17
    Serial.println("一个设备连接到了ESP32 WiFi");
    Serial.printf("当前设备数量：%d\n",WiFi.softAPgetStationNum());
    //isDeviceConnected = true;
    break;
  case SYSTEM_EVENT_AP_STADISCONNECTED://18
    Serial.println("一个设备断开和ESP32 WiFi连接");
    Serial.printf("当前设备数量：%d\n",WiFi.softAPgetStationNum());
    //isDeviceConnected = false;
    break;
  case SYSTEM_EVENT_AP_STAIPASSIGNED://19
    Serial.println("ESP32 WiFi为一个已连接设备分配了个IP");
    break;
  case SYSTEM_EVENT_AP_PROBEREQRECVED://20
    Serial.println("ESP32 WiFi从其他WiFi接收到一个请求(包)");
    break;
  case  SYSTEM_EVENT_ACTION_TX_STATUS://21  
    Serial.println("动作帧传输的接收状态");
    break;
  case SYSTEM_EVENT_ROC_DONE://22 
    Serial.println("Indicates the completion of Remain-on-Channel operation status");
    break;           
  case SYSTEM_EVENT_STA_BEACON_TIMEOUT://23   
    Serial.println("ESP32 WiFi信标超时");
    break;      
  case SYSTEM_EVENT_FTM_REPORT://24
    Serial.println("ESP32 WiFi接收到FTM程序报告");
    break;  
  case SYSTEM_EVENT_GOT_IP6://25
    Serial.println("ESP32 WiFi使用IPv6地址");
    break;
  case SYSTEM_EVENT_ETH_START://26
    Serial.println("ESP32 以太网启动");
    break;
  case SYSTEM_EVENT_ETH_STOP://27
    Serial.println("ESP32 以太网停止");
    break;
  case SYSTEM_EVENT_ETH_CONNECTED://28
    Serial.println("ESP32 以太网连接");
    break;
  case SYSTEM_EVENT_ETH_DISCONNECTED://29
    Serial.println("ESP32 以太网断开连接");
    break;
  case SYSTEM_EVENT_ETH_GOT_IP://30
    Serial.println("ESP32 以太网获取ip地址");
    break;
  case SYSTEM_EVENT_ETH_LOST_IP://31
    Serial.println("ESP32 以太网失去IP,IP被重置为0");
    break;
  default:
    break;
  }
}
