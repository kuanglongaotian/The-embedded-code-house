#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <WiFi.h>
#include <Arduino.h>
#include <esp_wifi.h>

u8_t Wifi_AP();
u8_t Wifi_Connect(const char *ssid,const char *password);
void MyWiFiEvent(WiFiEvent_t event);
void Wifi_Init();

#endif