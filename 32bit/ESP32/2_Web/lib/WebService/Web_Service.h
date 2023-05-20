#ifndef WEB_SERVICE_H
#define WEB_SERVICE_H

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <SPIFFS.h>

extern WebServer server;

void iniWebServer();
//String getContentType(String filename);

#endif
