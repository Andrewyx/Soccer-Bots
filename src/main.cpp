#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

#include <Wire.h>
#include <Adafruit_BusIO_Register.h>

#include "myasyncwebserver.h"
#include "mymotor.h"
#include "mylinesensor.h"
#include "myultrasonic.h"

//go to mymotor.cpp to change pins and stuff
//change your wifi password down below
//upload flash image via PLATFORM.IO/SPIFFS before uploading this
//control + click any functions to see its definition in VSCode


int rawIntData[4];

int A1PWM, A2PWM, B1PWM, B2PWM;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

void initWifiAP(){

  //change the below ssid and password to your own custom values
  const char* ssid = "AYM";
  const char* password = "andrewsboard";

  // Connect to Wi-Fi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  // Print ESP Local IP Address
  Serial.println(WiFi.softAPIP()); 

}

void initWiFiSTA() {

  //dont activate this unless you want a station connection, the AP (access point) method is a lot better for soccer bots
  const char* ssid = "McRoberts_Guest";
  const char* password = "mcrob6600";  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  initL298N();
  initSPIFFS();
  initWifiAP();
  initWebSocket();
  //These lines are for the soccer bot

  //initServoLib();
  //initLineSensor();
  //initWiFiSTA();
  //initUltrasonic();
  //these commented out lines are for an autonomous butler bot project. DO NOT COMMENT THEM IN

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.serveStatic("/", SPIFFS, "/");
  server.onNotFound(notFound);
  server.begin();

}

void loop() {
  ws.cleanupClients();
  runMotor();
}