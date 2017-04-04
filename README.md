# Lamp ESP

Flexible modular non-blocking system for controlling ESP8266.
You can reconfigure the system based on how you plug PINs.
You can turn on/off modules which are not used.
Configuration of current module is store in JSON file.

More information about the project:

- http://georgik.rocks/category/iot/

# Related project

- LampAndroidApp - https://github.com/georgik/LampAndroidApp - application which allows remot control of relay

# Core features

## Over the air update - OTA

Push new version to device over WiFi.

## MQTT Client

Connects to MQTT broker - send status and receive commands.

## Web server

Simple web interface which display's current status of module and let you configure whole system. You can also change the state of relay.

Configuration endpoint: `/config?NAME=VALUE`

Available configuration options:

 - `mqtt_host` - server host with Mosquitto or other MQTT broker
 - `mqtt_port` - server port with Mosquitto
 - `mqtt_parent_topic` - all topics for the devices will be published/subscribed under this topic (e.g. "/home")
 - `hostname` - device name used for publishing topics (e.g. "bathroom")
 - `temperature_correction` - correction which should be added to value measured by temperature sensor (e.g. -1.2)
 - `photocell_threshold` - value which is considered by photocell as light between 0 and 1023 (e.g. 200)
 - `pir_up_interval` - number of seconds when PIR is considered up after the first HIGH state
 - `dht_interval` - number of seconds between measurements

Reboot device: `/reboot`

## WiFiManager

Start Config Portal when device is not able to connect to WiFi.
Access 192.168.4.1 IP to configure the device.

# Customize application

Recommendation: Use PlatformIO for development.

You can turn on/off modules from code in LampMain.
Each module has setup function which takes two parameters or more:

 - `bool isEnabled`
 - `int pin`

Further configuration could be done via REST API and request to config page.
E.g.: `http://192.168.2.2/config?temperature_correction=-2`

# Suppored modules

## Air quality sensor

Read value from analog and send it to server.

## Display

Display value delivered to topic /+/+/display. E.g. temperature.
Current version is using SPI SSD1306 based on library:
https://github.com/squix78/esp8266-oled-ssd1306

## DHT sensor

Meassure temperature and humidity.

Schema: http://www.esp8266learning.com/wemos-mini-pir-sensor-example.php

## PIR sensor

Detect motion and change state of on-board led.

## Relay control

Receive MQTT command and change state of relay.

## RGB LED

RGB LED could be controlled via MQTT message sent to topic /parent/host/command.

Available colors:
 - `blue`
 - `green`
 - `orange`
 - `red`  
 - `white`

Or message `black` to turn the LED off.

Schema: http://www.esp8266learning.com/wemos-rgb-led-example.php

## Water Level Sensor

Indicate whether water was dete/restartcted by sensor

# Version 0.3

Intorduced modular architecture. User can configure functionality in top-level code.

# Version 0.2

Configuration is stored in config.json and managed by WiFiManager.

# Version 0.1

Direct communication based on GET API, MQTT and OTA support.
See branch v0.1.
