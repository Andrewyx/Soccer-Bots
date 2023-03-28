#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <math.h>

#include <sstream>
#include <string.h>
#include <fstream>

#include <json/value.h>
#include <json/json.h>

#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

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

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px; height: 100%; overflow: hidden;}
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

    <p style="text-align: center;">
        X: <span id="x_coordinate"> </span>
        Y: <span id="y_coordinate"> </span>
        Speed: <span id="speed"> </span> %
        Angle: <span id="angle"> </span>
    </p>
    <canvas id="canvas" name="game"></canvas>

   <script>
        var gateway = `ws://192.168.4.1/ws`;
        var connection;
        window.addEventListener('load', onLoad);

        function initWebSocket() {
          console.log('Trying to open a WebSocket connection...');
          connection = new WebSocket(gateway);
          connection.onopen    = onOpen;
          connection.onclose   = onClose;
        }        

        function onOpen(event) {
          console.log('Connection opened');
        }
        function onClose(event) {
          console.log('Connection closed');
          setTimeout(initWebSocket, 2000);
        }
        function onLoad(event) {
          initWebSocket();
        }        

        function toggleCheckbox(x) {
          var xhr = new XMLHttpRequest();
          xhr.open("GET", "/" + x, true);
          xhr.send();
        }

        function send(x,y,speed,angle){
            var data = {"x":x,"y":y,"speed":speed,"angle":angle};
            data = JSON.stringify(data);
            console.log(data);
            connection.send(data);
        }

        var canvas, ctx;

        window.addEventListener('load', () => {

            canvas = document.getElementById('canvas');
            ctx = canvas.getContext('2d');          
            resize(); 

            document.addEventListener('mousedown', startDrawing);
            document.addEventListener('mouseup', stopDrawing);
            document.addEventListener('mousemove', Draw);

            document.addEventListener('touchstart', startDrawing);
            document.addEventListener('touchend', stopDrawing);
            document.addEventListener('touchcancel', stopDrawing);
            document.addEventListener('touchmove', Draw);
            window.addEventListener('resize', resize);

            document.getElementById("x_coordinate").innerText = 0;
            document.getElementById("y_coordinate").innerText = 0;
            document.getElementById("speed").innerText = 0;
            document.getElementById("angle").innerText = 0;
        });

      
        var width, height, radius, x_orig, y_orig;
        function resize() {
            width = window.innerWidth;
            radius = 100;
            height = radius * 6.5;
            ctx.canvas.width = width;
            ctx.canvas.height = height;
            background();
            joystick(width / 2, height / 3);
        }

        function background() {
            x_orig = width / 2;
            y_orig = height / 3;

            ctx.beginPath();
            ctx.arc(x_orig, y_orig, radius + 20, 0, Math.PI * 2, true);
            ctx.fillStyle = '#ECE5E5';
            ctx.fill();
        }

        function joystick(width, height) {
            ctx.beginPath();
            ctx.arc(width, height, radius, 0, Math.PI * 2, true);
            ctx.fillStyle = '#F08080';
            ctx.fill();
            ctx.strokeStyle = '#F6ABAB';
            ctx.lineWidth = 8;
            ctx.stroke();
        }

        let coord = { x: 0, y: 0 };
        let paint = false;

        function getPosition(event) {
            var mouse_x = event.clientX || event.touches[0].clientX;
            var mouse_y = event.clientY || event.touches[0].clientY;
            coord.x = mouse_x - canvas.offsetLeft;
            coord.y = mouse_y - canvas.offsetTop;
        }

        function is_it_in_the_circle() {
            var current_radius = Math.sqrt(Math.pow(coord.x - x_orig, 2) + Math.pow(coord.y - y_orig, 2));
            if (radius >= current_radius) return true
            else return false
        }


        function startDrawing(event) {
            paint = true;
            getPosition(event);
            if (is_it_in_the_circle()) {
                ctx.clearRect(0, 0, canvas.width, canvas.height);
                background();
                joystick(coord.x, coord.y);
                Draw();
            }
        }


        function stopDrawing() {
            paint = false;
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            background();
            var data = {"x":0,"y":0,"speed":0,"angle":0};
            data = JSON.stringify(data);
            console.log(data);
            connection.send(data);

            joystick(width / 2, height / 3);
            document.getElementById("x_coordinate").innerText = 0;
            document.getElementById("y_coordinate").innerText = 0;
            document.getElementById("speed").innerText = 0;
            document.getElementById("angle").innerText = 0;
            
        }

        function Draw(event) {

            if (paint) {
                ctx.clearRect(0, 0, canvas.width, canvas.height);
                background();
                var angle_in_degrees,x, y, speed;
                var angle = Math.atan2((coord.y - y_orig), (coord.x - x_orig));

                if (Math.sign(angle) == -1) {
                    angle_in_degrees = Math.round(-angle * 180 / Math.PI);
                }
                else {
                    angle_in_degrees =Math.round( 360 - angle * 180 / Math.PI);
                }


                if (is_it_in_the_circle()) {
                    joystick(coord.x, coord.y);
                    x = coord.x;
                    y = coord.y;
                }
                else {
                    x = radius * Math.cos(angle) + x_orig;
                    y = radius * Math.sin(angle) + y_orig;
                    joystick(x, y);
                }

            
                getPosition(event);

                var speed =  Math.round(100 * Math.sqrt(Math.pow(x - x_orig, 2) + Math.pow(y - y_orig, 2)) / radius);

                var x_relative = Math.round(x - x_orig);
                var y_relative = Math.round(y - y_orig);
                

                document.getElementById("x_coordinate").innerText =  x_relative;
                document.getElementById("y_coordinate").innerText =y_relative ;
                document.getElementById("speed").innerText = speed;
                document.getElementById("angle").innerText = angle_in_degrees;

                send( x_relative,y_relative,speed,angle_in_degrees);
            }
          else if(!paint){

            send(0, 0, 0, 0);

          }
        } 
    </script>

  </body>
</html>)rawliteral";

int DegreeInRadian(double x){
  return x * (3.141592653589/180);;
}

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


void calcMotor(){
  int isForward = (int)(rawIntData[2] * sin(DegreeInRadian(rawIntData[3])));
  int isTurn = (int)(rawIntData[2] * cos(DegreeInRadian(rawIntData[3])));

  if(rawIntData[2] == 0 && rawIntData[3] == 0){
    A1PWM = 0;
    A2PWM = 0;
    B1PWM = 0;
    B2PWM = 0;
    isMoving = false;
  }
  else{
    isMoving = true;
  }

  if (isMoving){

    if(isForward > 0){
      A1PWM = map(isForward, 0, 100, 0, 255);
      A2PWM = 0;
      B1PWM = map(isForward, 0, 100, 0, 255);
      B2PWM = 0;      
    }
    else if(isForward < 0){
      A1PWM = 0;
      A2PWM = map(abs(isForward), 0, 100, 0, 255);
      B1PWM = 0;
      B2PWM = map(abs(isForward), 0, 100, 0, 255);      
    }

    /*
    if (isTurn > 0){
      B1PWM = map(isTurn, 0, rawIntData[2], 0, 255);
      B2PWM = 0;
    }
    else if(isTurn < 0){
      B1PWM = 0;
      B2PWM = map(abs(isTurn), 0, rawIntData[2], 0, 255);
    }
    */

  }
}

void printMotorValues(){
  Serial.print(A1PWM);
  Serial.print(A2PWM);
  Serial.print(B1PWM);
  Serial.println(B2PWM);
}

void runMotor(){
  calcMotor();
  analogWrite(MotorA1, A1PWM);
  analogWrite(MotorA2, A2PWM);
  analogWrite(MotorB1, B1PWM);
  analogWrite(MotorB2, B2PWM);  
}

void printGYRO(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Accelerometer ");
  Serial.print("X: ");
  Serial.print(a.acceleration.x, 1);
  Serial.print(" m/s^2, ");
  Serial.print("Y: ");
  Serial.print(a.acceleration.y, 1);
  Serial.print(" m/s^2, ");
  Serial.print("Z: ");
  Serial.print(a.acceleration.z, 1);
  Serial.print(" m/s^2 ||");

  Serial.print("Gyroscope ");
  Serial.print("X: ");
  Serial.print(g.gyro.x, 1);
  Serial.print(" rps, ");
  Serial.print("Y: ");
  Serial.print(g.gyro.y, 1);
  Serial.print(" rps, ");
  Serial.print("Z: ");
  Serial.print(g.gyro.z, 1);
  Serial.println(" rps");

  delay(100);
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
  ws.cleanupClients();
  //printGYRO();
  runMotor();
}