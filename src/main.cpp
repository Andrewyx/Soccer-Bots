#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "myasyncwebserver.h"
#include "mygyro.h"
#include "mymotor.h"

Adafruit_MPU6050 mpu;

// Replace with your network credentials
//const char* ssid = "McRoberts_Guest";
//const char* password = "mcrob6600";
//const char* ssid = "ThatOneHouse";
//const char* password = "Wz6960025";

const char* ssid = "AYM";
const char* password = "andrewsboard";

const int MotorA1 = 4;
const int MotorA2 = 18;
const int MotorB1 = 19;
const int MotorB2 = 23;

int rawIntData[4];

int A1PWM, A2PWM, B1PWM, B2PWM;

bool isMoving = false;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);

  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, LOW);

  initSPIFFS();

  if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU-6050 sensor");

  // Connect to Wi-Fi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Print ESP Local IP Address
  Serial.println(WiFi.softAPIP());

  initWebSocket();


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/afor", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(MotorA1, HIGH);
    digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, HIGH); 

    Serial.println("Motor On!");
    request->send(200, "text/plain", "ok");
  });
  server.on("/aback", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, HIGH);
    digitalWrite(MotorB2, LOW);  

    Serial.println("Motor Backwards!");
    request->send(200, "text/plain", "ok");
  });

  server.on("/off", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, LOW);  

    Serial.println("Motor Off!");
    request->send(200, "text/plain", "ok");
  });

  server.on("/left", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(MotorA1, LOW);
    digitalWrite(MotorA2, HIGH);
    digitalWrite(MotorB1, LOW);
    digitalWrite(MotorB2, HIGH);  

    Serial.println("Motor Turning Left!");
    request->send(200, "text/plain", "ok");
  });  

  server.on("/right", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(MotorA1, HIGH);
    digitalWrite(MotorA2, LOW);
    digitalWrite(MotorB1, HIGH);
    digitalWrite(MotorB2, LOW); 

    Serial.println("Motor Turning Right!");
    request->send(200, "text/plain", "ok");
  });

  
  server.onNotFound(notFound);
  server.begin();

}

void loop() {
  ws.cleanupClients();
  //printGYRO();
  runMotor();
}