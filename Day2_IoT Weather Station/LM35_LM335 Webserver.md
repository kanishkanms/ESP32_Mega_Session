## What is a Web Server?
A web server delivers content for a website to a client that requests it, like a web browser. The basic objective of the web server is to store, process and deliver web pages to the users. The communication between webserver and browser happens using HTTP - Hypertext Transfer Protocol. HTTP is a way to interact and fetch resources such as HTML files.

HTML (Hypertext Markup Language) is the code that is used to structure a web page and its content and is usually used inside a file (in our case, sketch).

## What is the difference?
 We will now record the values from the temperature sensor and display it on a webpage instead of serial monitor like in previous case.
 
 To achieve that, we will first connect esp32 to web browser over internet using <Wifi.h> library. Then using HTTP, we will create a simple webpage in HTML to display sensor values

## <Wifi.h> ??

This library allows our board to connect to the internet. It can serve as either a server accepting incoming connections or a client making outgoing ones. 
Find the documentation for the same [here](https://www.arduino.cc/en/Reference/WiFi)

To use this library

   ```
#include <WiFi.h>
  ```

### Code Explanation
The code for this has all the basics we previously learnt.
Additionally, we initialize variables and make instances for Wi-Fi name, Wi-Fi password 

   ```
const char *ssid =  "";     // Enter your WiFi Name
const char *pass =  ""; // Enter your WiFi Password
   ```

Next, we need to create a server that listens for incoming connections on the specified port. 80 is the default port for HTTP and so this value is used.
   ```
 WiFiServer server(80);
   ```
#### setup()
Set the pin mode, and initialize the serial monitor at a baud rate.

One important thing to keep in mind is to notice how we some other pin for connection here.
That's because ADC2 is used with WiFi, so analogRead() on ADC2 pins fails. ADC1 channels are saved to use ⇒ GPIO 32, 33, 34, 35.

Find more info [here](https://desire.giesecke.tk/index.php/2018/01/30/adc2-channel-cannot-be-used-when-wifi-is-in-use/)

We use .begin() function then connect the module with the Wi-Fi using WiFi.begin(ssid, password)
and ask serial monitor to print "." when not connected
 ```
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

```

Next we need to check whether any client has sent an HTTP request or not. If ESP32 gets any client request, then it will be stored in character and shown on the serial monitor.
```
WiFiClient client = server.available();
 if (client) 
  {                             
    Serial.println("new client");          
    String currentLine = "";                   //Storing the incoming data in the string
    while (client.connected()) 
    {            
      if (client.available()) 
      
 ```
Once the end of the request is reached, a webpage can be designed using HTML and client.print() function.
