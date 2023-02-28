#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "McRoberts_Guest";
const char* password = "mcrob6600";
//const char* ssid = "ThatOneHouse";
//const char* password = "Wz6960025";

const int MotorA1 = 18;
const int MotorA2 = 19;
const int MotorB1 = 21;
const int MotorB2 = 22;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2f4468;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #1f2e45}
      .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
    </style>
  </head>
  <body>
    <h1>ESP Pushbutton Web Server</h1>
    <button class="button" onmousedown="toggleCheckbox('afor');" ontouchstart="toggleCheckbox('afor');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">MOTOR FORWARD</button>
    <button class="button" onmousedown="toggleCheckbox('aback');" ontouchstart="toggleCheckbox('aback');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">MOTOR BACKWARD</button>
    <button class="button" onmousedown="toggleCheckbox('left');" ontouchstart="toggleCheckbox('left');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">LEFT</button>
    <button class="button" onmousedown="toggleCheckbox('right');" ontouchstart="toggleCheckbox('right');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">RIGHT</button>



   <script>
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
  </script>
  </body>
</html>)rawliteral";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
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


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Receive an HTTP GET request
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

}