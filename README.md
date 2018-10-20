# ESP8266 Captive Portal Phishing Example

_This code is for demonstration purposes only. It is not meant for any malicious intent._



WiFi captive portal meant for the NodeMCU ESP8266. It creates a WiFi with the name of your choosing. When an unsuspecting user joins the network, they are prompted with a captive portal which asks for a Email and an Email password input. When the user submits these informations to the server, they will be kept in the log located at `192.168.0.1/log.txt`. This log file will also contain information about all connections and disconnections, along with MAC addresses.




# Installation

To install this program to the ESP8266, it is necessary to install the [PString library](http://arduiniana.org/libraries/pstring/), the [Streaming library](http://arduiniana.org/libraries/streaming/), the [ESP8266 FS plugin](https://github.com/esp8266/arduino-esp8266fs-plugin) and of course the [ESP8266 Board](http://arduino.esp8266.com/stable/package_esp8266com_index.json) to the Arduino IDE. The filesystem plugin will have to be used for uploading the files in the data folder. Read on how to do this [here](https://github.com/esp8266/arduino-esp8266fs-plugin).



### Credits

Thanks to http://arduiniana.org for the PString library and the Streaming library. Also thanks to espressif for the arduino-esp8266fs-plugin and the esp8266-core. These helped a ton.



~~Happy hacking!~~

Happy _testing_!
