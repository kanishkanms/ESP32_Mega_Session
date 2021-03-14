#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino_JSON.h>
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid = "      ";
const char* pass = "      ";

// Create an ESP32 AsyncWebServer object on port 80(webpages)
AsyncWebServer server(80); 

// Create an Event Source on esp32 webpage 
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;
/* The JSON file sent to the server
{
  "gyroX":"0.02",
  "gyroY":"0.09",
  "gyroZ":"0.01",
}

*/

// Timer variables
unsigned long lastTime = 0;  
unsigned long gyroDelay = 10; // milliseconds

// Create an Adafruit object for MPU_6050 library
Adafruit_MPU6050 mpu;

sensors_event_t a, g, temp; // Define the sensor events (variables) to be retrieved from the sensor

float gyroX, gyroY, gyroZ;

//Gyroscope sensor deviation
float gyroXoffset = 0.07;
float gyroYoffset = 0.03;
float gyroZoffset = 0.01;

// Init MPU6050
void beginMPU(){
  if (!mpu.begin(0x69)) {
    Serial.println("Sensor Error! Wrong I2C Address");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 working! I2C initiated");
}

void beginSPIFFS() {
  if (!SPIFFS.begin()) // Open the 'data' file in SPIFFS plugin and check if there is any empty content.
  { 
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS data file retreived successfully");
}

// Initialize WiFi
void beginWiFi() {
  //WiFi.mode(WIFI_STA);  //Wifi in static mode(client mode) 
  WiFi.begin(ssid, pass);
  Serial.println("");
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.print("The IP Address: ");
  Serial.println(WiFi.localIP());
}

String getGyrovalues(){
  mpu.getEvent(&a, &g, &temp);

  float gyroX_temp = g.gyro.x;
  if(abs(gyroX_temp) > gyroXoffset)  // If the value is greater than the offset, assume that it is a valid reading
  {
    gyroX += gyroX_temp/50.00;  // theta_x_final = theta_x_initial + w_x*t
  }
  
  float gyroY_temp = g.gyro.y;
  if(abs(gyroY_temp) > gyroYoffset) {
    gyroY += gyroY_temp/70.00;
  }

  float gyroZ_temp = g.gyro.z;
  if(abs(gyroZ_temp) > gyroZoffset) {
    gyroZ += gyroZ_temp/90.00;
  }

  readings["gyroX"] = String(gyroX); // Add the contents in string format of the data to be sent
  readings["gyroY"] = String(gyroY);
  readings["gyroZ"] = String(gyroZ);

  String gyroString = JSON.stringify(readings); // Convert the contents to a JSON file using JSON.stringify
  return gyroString; // return type is a JSON file with string values
  /*
  gyroString = {"accX":"0.08", "accY":"0.02", "accZ":"0.12"}
  */
}

void setup() {
  Serial.begin(115200);
  beginWiFi();
  beginSPIFFS();
  beginMPU();

  // Handle Web Server
  /*
   1st arg - pass a string with a route where the server will be listening for incoming requests
   2nd arg - HTTP methods allowed on that route. The server is just getting values, only allow HTTP_GET method
   */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) // Setup the server at default url "<ip-addr>/"
  {
    request->send(SPIFFS, "/index.html", "text/html"); // Whenever the client 'request' for the server webpage, show the index.html page
  });

  server.serveStatic("/", SPIFFS, "/"); // the url would be "<ip-addr>/"

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) // The url would be "<ip-addr>/reset"
  {
    gyroX=0;
    gyroY=0;
    gyroZ=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetX", HTTP_GET, [](AsyncWebServerRequest *request) // The url would be "<ip-addr>/resetX"
  {
    gyroX=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetY", HTTP_GET, [](AsyncWebServerRequest *request) // The url would be "<ip-addr>/resetY"
  {
    gyroY=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetZ", HTTP_GET, [](AsyncWebServerRequest *request) // The url would be "<ip-addr>/resetZ"
  {
    gyroZ=0;
    request->send(200, "text/plain", "OK");
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client) // Describes what happens initially when client connects to server
  {
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  server.begin();
}

void loop() 
{
  if ((millis() - lastTime) > gyroDelay) // Send data(JSON) after every 10 milliseconds
  {
    // Send Events to the Web Server with the Sensor Readings
    // Name of the event is "gyro_readings"
    events.send(getGyrovalues().c_str(),"gyro_readings",millis()); // .c_str() converts the String values to chars as .send() function accepts char values only
    lastTime = millis();
  }
}
