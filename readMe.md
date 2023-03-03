#                                                                    #
#        ESP32CAM Local Host Control and Time Display README         #
#                                                                    #

This code allows you to display current time, turn on lights using a local host, and it is not accessible remotely. 

Components:
- WiFi.h
- Wire.h
- LiquidCrystal_I2C.h
- NTPClient.h
- WiFiUdp.h
- WiFiClient.h
- ESPAsyncWebServer.h
- AsyncTCP.h

To use this code, you need to replace the network credentials:
const char* ssid = "Your Network Name";
const char* password = "Your Network Password";

Then, you need to define the pins for the lights:
const int lightPin1 = 4;
const int lightPin2 = 5;

Afterward, initialize the Serial Monitor, Wi-Fi network, LCD, and NTP Client.

There are three routes:
1. "/" route displays the time, allows you to enter a message, and turn on/off the lights.
2. "/send-message" route sends a message to the LCD screen.
3. "/turn-on-lights" and "/turn-off-lights" routes turn on/off the lights.

Note: You can access the web page at http://<ESP32CAM_IP_address>:81/ using a web browser.

To run this code, you need to install the following libraries:
- AsyncTCP
- ESPAsyncWebServer
- LiquidCrystal_I2C
- NTPClient

To build the code, use the Arduino IDE, and make sure to select the correct board and port.

           ###############################################
           #                                             #
           #        ESP32CAM Time Display README         #
           #                                             #
           ###############################################

