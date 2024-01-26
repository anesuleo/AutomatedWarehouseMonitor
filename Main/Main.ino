// This code is derived from the HelloServer Example 
// in the (ESP32) WebServer library .
//
// It hosts a webpage which has one temperature reading to display.
// The webpage is always the same apart from the reading which would change.
// The getTemp() function simulates getting a temperature reading.
// homePage.h contains 2 constant string literals which is the two parts of the
// webpage that never change.
// handleRoot() builds up the webpage by adding as a C++ String:
// homePagePart1 + getTemp() +homePagePart2 
// It then serves the webpage with the command:  
// server.send(200, "text/html", message);
// Note the text is served as html.
//
// Replace the code in the homepage.h file with your own website HTML code.
// 
// This example requires only an ESP32 and download cable. No other hardware is reuired.
// A wifi SSID and password is required.
// Written by: Natasha Rohan  12/3/23
//
#include <Wire.h>
#include "rgb_lcd.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "homepage.h"
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 18
rgb_lcd lcd;

const char* ssid = "Leo";
const char* password = "njove123";
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;
const int trigPin = 17;
const int echoPin = 16;
long duration;
float cms, inches;
//Left Motor
const int MOTOR_L_PIN = 33;
const int ENABLE_L_PIN = 32;

//Right Motor
const int MOTOR_R_PIN = 27;
const int ENABLE_R_PIN = 14;

//Setting PWM properties
const int FREQ = 30000;
const int PWM_CHANNEL = 0;
const int RESOLUTION = 8;
int dutyCycle = 0;

WebServer server(80);

//temp function to simulate temp sensor
String getTemp() {
  DHT.read(DHT11_PIN);
  String temp = String(DHT.temperature);
  return temp;
}

String getHumid()
{
  DHT.read(DHT11_PIN);
  String humid = String(DHT.humidity);
  return humid;
}

void handleRoot() {
  String message = homePagePart1 + getTemp() + homePagePart2 + getHumid() + homePagePart3;
  server.send(200, "text/html", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}

void setup(void) {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
    Serial.begin(115200);
  pinMode(MOTOR_R_PIN, OUTPUT);
  pinMode(MOTOR_L_PIN, OUTPUT);
  ledcSetup(PWM_CHANNEL, FREQ, RESOLUTION);
  ledcAttachPin(ENABLE_R_PIN, PWM_CHANNEL);
  ledcAttachPin(ENABLE_L_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, dutyCycle);
  //Ultrasound SetUp
 
  //LCD Setup
  
  lcd.begin(16, 2);
    
  lcd.setRGB(colorR, colorG, colorB);

  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
   
  //LCD Display
  lcd.setCursor(0, 0);
  lcd.print("Temp: " + String(DHT.temperature));
  lcd.setCursor(0, 1);
  lcd.print("Humidity: " + String(DHT.humidity));
  
  //Ultrasound
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  cms = duration * 0.034 / 2;
  inches = (duration/2) / 74;
  // Prints the distance on the Serial Monitor

  dutyCycle = 200;
  ledcWrite(PWM_CHANNEL, dutyCycle);
  if (cms > 5)
  {
    forward();
  }
  else
  {
    stop();
    delay(1000);
    reverse();
    delay(3000);
    right();
    delay(1000);
  }

  if (Serial.available() > 0) {
    // Read the incoming byte
    char incomingByte = Serial.read();

    // Print the received byte
    Serial.print("Received: ");
    Serial.println(incomingByte);


    // Check if the received byte is '1'
    if (incomingByte == '1') {
      // Call the forward() function
     dutyCycle = 200;
     ledcWrite(PWM_CHANNEL, dutyCycle);
      forward();
      delay(3000);
      right();
      delay(500);
      forward();
      delay(1500);
      stop();
    }
     if (incomingByte == '2') {
      // Call the forward() function
     dutyCycle = 200;
     ledcWrite(PWM_CHANNEL, dutyCycle);
      forward();
      delay(3000);
      left();
      delay(500);
      forward();
      delay(1500);
      stop();
    }
  }
 
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
