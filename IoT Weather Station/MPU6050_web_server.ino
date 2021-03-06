#include <Wire.h>
#include <WiFi.h>
const int MPU_addr=0x69;  // I2C address of the MPU-6050
int16_t Tmp;

// WiFi network info.
const char *ssid =  "Nokia 8.1";     // Enter your WiFi Name
const char *pass =  "Swathi123456789"; // Enter your WiFi Password
WiFiServer server(80);

void setup(){
 Serial.begin(115200);
 Wire.begin();
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x6B);  // PWR_MGMT_1 register
 Wire.write(0);     // set to zero (wakes up the MPU-6050)
 Wire.endTransmission(true);
 Serial.println("Wrote to IMU");
 Serial.println("Connecting to ");
 Serial.println(ssid);
 WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");              // print ... till not connected
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address is : ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");
}
void loop(){
mpu_read();
WiFiClient client = server.available();
 if (client) 
  {                             
    Serial.println("new client");          
    String currentLine = "";                   //Storing the incoming data in the string
    while (client.connected()) 
    {            
      if (client.available())                  //if there is some client data available
      {                
        char c = client.read();                // read a byte
          if (c == '\n')                       // check for newline character, 
          {                     
          if (currentLine.length() == 0)      //if line is blank it means its the end of the client HTTP request
          {     
            client.print("<html><title> ESP32 WebServer</title></html>");
            client.print("<body bgcolor=\"#E6E6FA\"><h1 style=\"text-align: center; color: blue\"> ESP32 WebServer </h1>");     
            client.print("<p style=\"text-align: left; color: red; font-size:150% \">Temperature Value: ");
            client.print("<p style=\"text-align: left; font-size:150% \">Tmp: "); 
            client.print(Tmp/340.00+36.53);
            client.print("</p></body>");   
            break;  // break out of the while loop:
          } 
           else
          {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
         } 
         else if (c != '\r') 
         {  // if you got anything else but a carriage return character,
          currentLine += c;       // add it to the end of the currentLine
         }
        }
      }
  }
}
void mpu_read(){
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
 Wire.endTransmission(false);
 Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers

 Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

 Serial.print("\nTemperature: " );  Serial.print(Tmp/340.00+36.53);
 Serial.print("\n");
 delay(3000);
 }
