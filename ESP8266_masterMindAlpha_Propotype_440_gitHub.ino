// CannaIoT masterMindAlpha Prototype440 ESP8266 programm.
// 
// This is a programm for ESP8266 for the CannaIoT maserMindAplha device.
// Allows to connect to local Wi-Fi network and send data between Arduino 
// and user interface via web-page on ESP8266.
//
// (Under developement...) 
//



#include <ESP8266WiFi.h>

// Your network
const char* ssid     = "********"; // Put your Wi-Fi name here
const char* password = "********";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

bool flag;

// for testing
bool handshakeGiven = 0;
bool messageRecieved = 0;

// Variables for Day Time

int dayTimeStartHours;
int dayTimeStartMinutes;

float minDayTemp;
float maxDayTemp;

float minDayHumid;
float maxDayHumid;

// Variables for Night Time:

int nightTimeStartHours;
int nightTimeStartMinutes;

float minNightTemp;
float maxNightTemp;

float minNightHumid;
float maxNightHumid;

String incomingString = "";
String valueString = "";
  
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


// Set your Static IP address
IPAddress local_IP(192, 168, 8, 185);
// Set your Gateway IP address
IPAddress gateway(192, 168, 8, 1);
IPAddress subnet(255, 255, 255, 0);


void setup() {

  flag = 0;
  
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println("CannaIoT masterMindAlpha ESP8266 status: ON");

 //Check if device configuration is ok
 
 if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Device network configuration failed");
  }
  
  // Connect to Wi-Fi network with SSID and password
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("...");
    delay(500);
  }
  
  Serial.print("Connected to Wi-Fi: ");
  Serial.println(ssid);
  Serial.print("Your local IP:");
  Serial.println(WiFi.localIP());
  server.begin();
}


void loop(){
  

//  Ask masterMindAlpha for actual values set in the device

  while (flag == 1){
  flag = 0;
  delay(100);
  Serial.println("valuesUpdate");
}

  serialUpdate();

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
         header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


// To set variables (parameters) via web-page we will create a simple HTML form.
// After submitting the form, we will read the header and search for a string 
// with variable name and value.
//
// 1. If any string is found in the header after the "GET" action of form submit, 
//  search if there is a string corresponding to value set in the form.
// 2. Find the index of header where the variable (string) starts, 
// 3. Move to where is the last index of the string plus one more 
// (as after the variable in the header there is "=").
// 4. Take next two characters and make a string of it 
// (this will be the submitted value of the variable).
// 5. Change the variable from string to integer (int) or float.
// 6. Send a message via ESP8266 Serial Output (TX pin) to masterMindAlpha device
// (Arduino RX pin). The message will start with "set". eg. "setDayStartHour".
// 7. MasterMindAlpha will read the message and set the value submitted in the form.


// Serial Print first 100 characters of header (for testing purpouse)
//
//              for (int i = 0; i <= 100; i++){
//                Serial.print(i);
//                Serial.println(header[i]);
//              }

 
            if (header.indexOf("dayTimeStartHours") >= 0) {
              int index = header.indexOf("dayTimeStartHours");
              String dayHour = "";
              dayHour += header[index+18];
              dayHour += header[index+19];
              
              int dayHourInt = dayHour.toInt();
              dayTimeStartHours = dayHourInt;
              delay(100);
              Serial.print("setDayTimeStartHours ");
              Serial.println(dayTimeStartHours);
              delay(1000);
              Serial.println();   
            }

            if (header.indexOf("dayTimeStartMinutes") >= 0) {
              int index = header.indexOf("dayTimeStartMinutes");
              String dayMinutes = "";
              dayMinutes += header[index+20];
              dayMinutes += header[index+21];

              int dayMinutesInt = dayMinutes.toInt();
              dayTimeStartMinutes = dayMinutesInt;
              delay(100);
              Serial.print("setDayTimeStartMinutes ");
              delay(100);
              Serial.println(dayTimeStartMinutes);
              delay(1000);
              Serial.println(); 
            }

            if (header.indexOf("minDayTemp") >= 0) {
              int index = header.indexOf("minDayTemp");
              String minDayT = "";
              minDayT += header[index+11];
              minDayT += header[index+12];

              float minDayTFloat = minDayT.toFloat();
              minDayTemp = minDayTFloat;
              delay(100);
              Serial.print("setMinDayTemp ");
              delay(100);
              Serial.println(minDayTemp);
              delay(1000);
              Serial.println(); 
            }

            if (header.indexOf("maxDayTemp") >= 0) {
              int index = header.indexOf("maxDayTemp");
              String maxDayT = "";
              maxDayT += header[index+11];
              maxDayT += header[index+12];

              float maxDayTFloat = maxDayT.toFloat();
              maxDayTemp = maxDayTFloat;
              delay(100);
              Serial.print("setMaxDayTemp ");
              delay(100);
              Serial.println(maxDayTemp);
              delay(1000);
              Serial.println(); 
            }

            if (header.indexOf("minDayHumid") >= 0) {
              int index = header.indexOf("minDayHumid");
              String minDayH = "";
              minDayH += header[index+12];
              minDayH += header[index+13];

              float minDayHFloat = minDayH.toFloat();
              minDayHumid = minDayHFloat;
              delay(100);
              Serial.print("setMinDayHumid ");
              delay(100);
              Serial.println(minDayHumid);
              delay(1000);
              Serial.println(); 
            }

            if (header.indexOf("maxDayHumid") >= 0) {
              int index = header.indexOf("maxDayHumid");
              String maxDayH = "";
              maxDayH += header[index+12];
              maxDayH += header[index+13];

              float maxDayHFloat = maxDayH.toFloat();
              maxDayHumid = maxDayHFloat;
              delay(100);
              Serial.print("setMaxDayHumid ");
              delay(100);
              Serial.println(maxDayHumid);
              delay(1000);
              Serial.println(); 
            }

// Form for submitting Night Settings

              if (header.indexOf("nightTimeStartHours") >= 0) {
              int index = header.indexOf("nightTimeStartHours");
              String nightHour = "";
              nightHour += header[index+20];
              nightHour += header[index+21];
              
              int nightHourInt = nightHour.toInt();
              nightTimeStartHours = nightHourInt;
              delay(100);
              Serial.print("setNightTimeStartHours");
              Serial.println(nightTimeStartHours);
              delay(1000);
              Serial.println();   
            }

            if (header.indexOf("nightTimeStartMinutes") >= 0) {
              int index = header.indexOf("nightTimeStartMinutes");   
              String nightMinutes = "";
              nightMinutes += header[index+22];
              nightMinutes += header[index+23];

              int nightMinutesInt = nightMinutes.toInt();
              nightTimeStartMinutes = nightMinutesInt;
              delay(100);
              Serial.print("setNightTimeStartMinutes ");
              Serial.println(nightTimeStartMinutes);
              delay(1000);
              Serial.println(); 
            }

            if (header.indexOf("minNightTemp") >= 0) {
              int index = header.indexOf("minNightTemp");
              String minNightT = "";
              minNightT += header[index+13];
              minNightT += header[index+14];

              float minNightTFloat = minNightT.toFloat();
              minNightTemp = minNightTFloat;
              delay(100);
              Serial.print("setMinNightTemp ");
              delay(100);
              Serial.println(minNightTemp);
              delay(1000);
              Serial.println(); 
            }

            if (header.indexOf("maxNightTemp") >= 0) {
              int index = header.indexOf("maxNightTemp");
              String maxNightT = "";
              maxNightT += header[index+13];
              maxNightT += header[index+14];

              float maxNightTFloat = maxNightT.toFloat();
              maxNightTemp = maxNightTFloat;
              delay(100);
              Serial.print("setMaxNightTemp ");
              delay(100);
              Serial.println(maxNightTemp);
              delay(1000);
              Serial.println(); 
            }

            if (header.indexOf("minNightHumid") >= 0) {
              int index = header.indexOf("minNightHumid");
              String minNightH = "";
              minNightH += header[index+14];
              minNightH += header[index+15];

              float minNightHFloat = minNightH.toFloat();
              minNightHumid = minNightHFloat;
              delay(100);
              Serial.print("setMinNightHumid ");
              delay(100);
              Serial.println(minNightHumid);
              delay(1000);
              Serial.println(); 
            }

            if (header.indexOf("maxNightHumid") >= 0) {
              int index = header.indexOf("maxNightHumid");
              String maxNightH = "";
              maxNightH += header[index+14];
              maxNightH += header[index+15];

              float maxNightHFloat = maxNightH.toFloat();
              maxNightHumid = maxNightHFloat;
              delay(100);
              Serial.print("setMaxNightHumid ");
              delay(100);
              Serial.println(maxNightHumid);
              delay(1000);
              Serial.println(); 
            }

    // To update values on demand

            if (header.indexOf("values") >= 0) {             
             Serial.println("handshake");  
            }       
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; background-color: darkolivegreen; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("</style></head>");
            
           
            client.println("<body><h1>CannaIoT masterMindAlpha Control Server - Prototype440</h1>");
            
            client.println("<p><a href=\"/?values\"><button>UPDATE VALUES</button></a></p>");
             
             // HTML form for Day Settings:     
             
                  client.println("<br><br><br>");
                  client.println("<form action=\"\">");
                  client.println("<fieldset>");
                  client.println("<legend>Settings For Day</legend>");
                  client.println("<label for=\"dayTimeStartHours\">Day Time Start Hour: </label>");
                  client.print("<input  type=\"text\" id=\"dayTimeStartHours\" name=\"dayTimeStartHours\" value=");
                  client.print(dayTimeStartHours);
                  client.println("><br><br>");
                  client.println("<label for=\"dayTimeStartMinutes\">Day Time Start Minutes:</label>");
                  client.println("<input type=\"text\" id=\"dayTimeStartMinutes\" name=\"dayTimeStartMinutes\" value=");
                  client.print(dayTimeStartMinutes);
                  client.println("><br><br>");
                  client.println("<label for=\"minDayTemp\">Day Time Minimal Temperature: </label>");
                  client.print("<input type=\"text\" id=\"minDayTemp\" name=\"minDayTemp\" value=");
                  client.print(minDayTemp);
                  client.println("><br><br>");
                  client.println("<label for=\"maxDayTemp\">Day Time Maximal Temperature</label>");
                  client.print("<input type=\"text\" id=\"maxDayTemp\" name=\"maxDayTemp\" value=");
                  client.print(maxDayTemp);
                  client.println("><br><br>");
                  client.println("<label for=\"minDayHumid\">Day Time Minimal Humidity</label>");
                  client.print("<input type=\"text\" id=\"minDayHumid\" name=\"minDayHumid\" value=");
                  client.print(minDayHumid);
                  client.println("><br><br>");
                  client.println("<label for=\"maxDayHumid\">Day Time Maximal Humidity</label>");
                  client.print("<input type=\"text\" id=\"maxDayHumid\" name=\"maxDayHumid\" value=");
                  client.print(maxDayHumid);
                  client.println("><br><br>");
                  client.println("<input type=\"submit\" value=\"Submit\">");
                  client.println("</fieldset>");
                  client.println("</form>");
            
             // HTML form for Night Settings:  
             
                  client.println("<br><br><br>");
                  client.println("<form action=\"\">");
                  client.println("<fieldset>");
                  client.println("<legend>Settings For Night</legend>");
                  client.println("<label for=\"nightTimeStartHours\">Night Time Start Hour: </label>");
                  client.print("<input  type=\"text\" id=\"nightTimeStartHours\" name=\"nightTimeStartHours\" value=");
                  client.print(nightTimeStartHours);
                  client.println("><br><br>");
                  client.println("<label for=\"nightTimeStartMinutes\">Night Time Start Minutes:</label>");
                  client.println("<input type=\"text\" id=\"nightTimeStartMinutes\" name=\"nightTimeStartMinutes\" value=");
                  client.print(nightTimeStartMinutes);
                  client.println("><br><br>");
                  client.println("<label for=\"minNightTemp\">Night Time Minimal Temperature: </label>");
                  client.print("<input type=\"text\" id=\"minNightTemp\" name=\"minNightTemp\" value=");
                  client.print(minNightTemp);
                  client.println("><br><br>");
                  client.println("<label for=\"maxNightTemp\">Night Time Maximal Temperature</label>");
                  client.print("<input type=\"text\" id=\"maxNightTemp\" name=\"maxNightTemp\" value=");
                  client.print(maxNightTemp);
                  client.println("><br><br>");
                  client.println("<label for=\"minNightHumid\">Night Time Minimal Humidity</label>");
                  client.print("<input type=\"text\" id=\"minNightHumid\" name=\"minNightHumid\" value=");
                  client.print(minNightHumid);
                  client.println("><br><br>");
                  client.println("<label for=\"maxNightHumid\">Night Time Maximal Humidity</label>");
                  client.print("<input type=\"text\" id=\"maxNightHumid\" name=\"maxNightHumid\" value=");
                  client.print(maxNightHumid);
                  client.println("><br><br>");
                  client.println("<input type=\"submit\" value=\"Submit\">");
                  client.println("</fieldset>");
                  client.println("</form>");

                  

// Check if there was a handshake response from materMindAlpha
// (if there was a response handshakeGiven will be "1")

                  client.println("<br><br><br><div>masterMindAlpha response: ");
                  client.println(handshakeGiven);
                  client.println("</div><br><br>");

                  
                  client.println("<br><br><br><div>setDayTimeHours recieved: ");
                  client.println(messageRecieved);
                  client.println("</div><br><br>");
      
            
        
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
  }
}

// Values from masterMindAlpha

void serialUpdate(){
  
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    incomingString += (char)inChar;

    // if you get a newline, print the string:
    if (inChar == '\n') {
      incomingString = "";
    }

    if (incomingString == "hello") {
    Serial.println("Hello! This is CannaIoT masterMindAlpha ESP8266. (ver. Prototype440)");   
    }

  // check if masterMindAlpha responses

    if (incomingString == "handshake") {
    delay(20);
    handshakeGiven = 1;
    flag = 1; 
    }
 
    
// When masterMindAlpha (Arduino) recieves the message "valuesUpdate" it responses
// with sending back each parameter with value. ESP8266 will read those messages 
// and if correspoding string is found, the value will be set.
// (under developement)
// (Maybe, this will be later replaced using JSON object).
   
    //Day Start Time Hours
    if (incomingString == "setDayTimeStartHours") {
      delay(20);
        messageRecieved = 1;
        while (Serial.available() > 0) {
        char inChar = Serial.read();
        valueString += (char)inChar;
        if (inChar == '\n') {
          dayTimeStartHours = valueString.toInt();
          // clear the string for new input:
          valueString = "";
          incomingString = "";
        }
      }
    }  
  }  
}
