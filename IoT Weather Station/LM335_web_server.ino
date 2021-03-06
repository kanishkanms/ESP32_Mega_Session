#include <WiFi.h> 

const int Temp_Pin = 32;
float Raw_Val;
float Voltage_Out;

float temp_C, temp_F, temp_K;

// WiFi network info.
const char *ssid =  "Nokia 8.1";     // Enter your WiFi Name
const char *pass =  "Swathi123456789"; // Enter your WiFi Password

WiFiServer server(80);

void setup(){
 pinMode(32, INPUT); //Above code's setup part
 Serial.begin(115200);

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
 
  Raw_Val = analogRead(Temp_Pin); //Above code's loop part
  //Serial.print(Raw_Val);
  Voltage_Out = (Raw_Val * 3300) / (4096);

  // calculate temperature for LM35 (LM35DZ)
  //temp_C = Voltage_Out/ 10;
  //temp_F = (temp_C * 1.8) + 32;

  // calculate temperature for LM335
  temp_K = Voltage_Out / 10;
  temp_C = temp_K - 273;
  temp_F = (temp_C * 1.8) + 32;
  //return temp_C;
  
  Serial.print("Temperature(ºC): ");
  Serial.print(temp_C);
  Serial.print("  Temperature(ºF): ");
  Serial.print(temp_F);
  Serial.print("  Voltage(mV): ");
  Serial.println(Voltage_Out);
  delay(1000);

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
  
            client.print("<p style=\"text-align: left; color: red; font-size:150% \">Temperature Value LM335: ");
            client.print("<p style=\"text-align: left; font-size:150% \">Temprature: "); 
            client.print(temp_C);
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
