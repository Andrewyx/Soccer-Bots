#include <Arduino.h>

#include <sstream>
#include <string.h>
#include <fstream>

#include <json/value.h>
#include <json/json.h>

#include "myasyncwebserver.h"


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
//use this
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {

      //std::istream jsonString(data);
      Json::Reader reader;
      Json::Value cleanedData; 
      char* dataAsString = (char*)data; 
  
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str(), data);
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      //handle data here
      //Serial.printf("[%u] Data: %s\n", client, data);
      bool parseSuccess = reader.parse(dataAsString, cleanedData, false);

      if(parseSuccess) {
        Json::Value xValue = cleanedData["x"];
        Json::Value yValue = cleanedData["y"];
        Json::Value speedValue = cleanedData["speed"];
        Json::Value angleValue = cleanedData["angle"];
        Serial.printf("[%u] X: [%d] Y:[%d] Speed:[%d] Angle[%d]\n", client, xValue.asInt(), 
              yValue.asInt(), speedValue.asInt(), angleValue.asInt());
        rawIntData[0] = xValue.asInt();
        rawIntData[1] = yValue.asInt();
        rawIntData[2] = speedValue.asInt();
        rawIntData[3] = angleValue.asInt();

      }
      else {
        Serial.printf("Error: [%u]", reader.getFormattedErrorMessages());
      }      
      break;
      
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}