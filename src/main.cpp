#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

#include <Wire.h>
#include <Adafruit_BusIO_Register.h>

#include "myasyncwebserver.h"
#include "mymotor.h"

const int MotorA1 = 4;
const int MotorA2 = 18;
const int MotorB1 = 19;
const int MotorB2 = 23;

const int lineSensor1 = 16;

int rawIntData[4];

int A1PWM, A2PWM, B1PWM, B2PWM;
bool isMoving = false;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

void initWifiAP(){
  const char* ssid = "AYM";
  const char* password = "andrewsboard";
  // Connect to Wi-Fi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  // Print ESP Local IP Address
  Serial.println(WiFi.softAPIP()); 

}

void initWiFiSTA() {
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

  //initL298N();
  initSPIFFS();
  initServoLib();
  //initWiFiSTA();
  initWifiAP();
  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.serveStatic("/", SPIFFS, "/");
  server.onNotFound(notFound);
  server.begin();

}

void loop() {
  ws.cleanupClients();
  runButlerMotor();
  //runMotor();
  
}