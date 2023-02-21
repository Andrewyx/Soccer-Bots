/*  Arduino DC Motor Control - PWM | H-Bridge | L298N  -  Example 01

    by Dejan Nedelkovski, www.HowToMechatronics.com
*/

//#define enA 
#define MotorA1 18
#define MotorA2 19
#define MotorB1 21
#define MotorB2 22

#define potPin 32
//#define button 4
//#define enR 10

#include <WiFi.h>

const char* ssid = "McRoberts_Guest";
const char* password = "mcrob6600";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
int rotDirection = 0;
int pressed = false;

int potValue;
int pwmOutput;
bool forward = false;

void setup() {

  analogWrite(MotorA1, 0);
  analogWrite(MotorA2, 255);
  analogWrite(MotorB1, 0);
  analogWrite(MotorB2, 255);

  Serial.begin(9600);
}

void loop() {

  potValue = analogRead(A0); // Read potentiometer value
  pwmOutput = map(potValue, 0, 4096, 0 , 255); // Map the potentiometer value from 0 to 255

  if (forward){
    for (int i; i<255; i++){
      analogWrite(MotorA1, 0);
      analogWrite(MotorB1, 0);
      
      analogWrite(MotorA2, i);
      analogWrite(MotorB2, i);
      delay(100);
    }
    forward = !forward;
  }
  if (!forward){
    for (int x; x < 255; x++){
      analogWrite(MotorA2, 0);
      analogWrite(MotorB2, 0);

      analogWrite(MotorB1, x);
      analogWrite(MotorA1, x);
      
      delay(100);
    }
    forward = !forward;
  }  
}