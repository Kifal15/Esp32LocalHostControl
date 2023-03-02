#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

// Replace with your network credentials
const char* ssid = "Saadat";
const char* password = "03118549774";

// Define the NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);

// Create an instance of the web server
AsyncWebServer server(81);

// Create an instance of the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define the pins for the lights
const int lightPin1 = 4;
const int lightPin2 = 5;
void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi reconnected");
  }
}
void setup(){
  pinMode(lightPin1, OUTPUT);
pinMode(lightPin2, OUTPUT);

  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Connect to Wi-Fi network with SSID and password
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize NTP Client
  timeClient.begin();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><head><title>ESP32CAM Time Display</title>";
    html += "<link href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css' rel='stylesheet'>";
    html += "<style>body{background-color: #212529; color: #fff;}</style>";
    html += "</head><body>";
    html += "<div class='container'><div class='row'>";
    html += "<div class='col-md-12'><h1 class='text-center'>ESP32CAM Time Display</h1></div>";
    html += "<div class='col-md-12'><h2 class='text-center'>" + getTime() + "</h2></div>";
    html += "<div class='col-md-12'><h3 class='text-center'>Enter a message:</h3></div>";
    html += "<div class='col-md-12'><input type='text' class='form-control' id='message' placeholder='Type your message here'></div>";
    html += "<div class='col-md-12'><br><button class='btn btn-primary btn-lg btn-block' onclick='sendMessage()'>Send Message</button></div>";
    html += "<div class='col-md-12'><br><button class='btn btn-success btn-lg btn-block' onclick='turnOnLights()'>Turn On Lights</button></div>";
    html += "<div class='col-md-12'><br><button class='btn btn-danger btn-lg btn-block' onclick='turnOffLights()'>Turn Off Lights</button></div>";
    html += "</div></div>";
    html += "<script>function sendMessage(){var message = document.getElementById('message').value;fetch('/send-message?message=' + message);}function turnOnLights(){fetch('/turn-on-lights');}function turnOffLights(){fetch('/turn-off-lights');}</script>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  // Route for sending a message
// Route for sending a message
server.on("/send-message", HTTP_GET, [](AsyncWebServerRequest *request){
String message;
if (request->hasParam("message")) {
message = request->getParam("message")->value();
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Message:");
lcd.setCursor(0, 1);
lcd.print(message);
}
request->send(200, "text/plain", message);
});

// Route for turning on the lights
server.on("/turn-on-lights", HTTP_GET, [](AsyncWebServerRequest *request){
digitalWrite(lightPin1, HIGH);
digitalWrite(lightPin2, HIGH);
request->send(200, "text/plain", "Lights turned on");
});

// Route for turning off the lights
server.on("/turn-off-lights", HTTP_GET, [](AsyncWebServerRequest *request){
digitalWrite(lightPin1, LOW);
digitalWrite(lightPin2, LOW);
request->send(200, "text/plain", "Lights turned off");
});

// Start the web server
server.begin();
}

void loop(){
    checkWiFi();

// Update NTP client and get the time
timeClient.update();
String time = getTime();

// Display the time on the LCD
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Time:");
lcd.setCursor(0, 1);
lcd.print(time);

// Wait for one second before updating again
delay(1000);
}

String getTime() {
String formattedTime = timeClient.getFormattedTime();
formattedTime = formattedTime.substring(0, formattedTime.length() - 3);
return formattedTime;
}