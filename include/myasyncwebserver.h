#ifndef MYASYNCWEBSERVER_h
#define MYASYNCWEBSERVER_h
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

extern int rawIntData[4];
extern AsyncWebServer server;
extern AsyncWebSocket ws;

void notFound(AsyncWebServerRequest *request);

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len);

void initWebSocket();

#endif