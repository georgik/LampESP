# Lamp ESP module

ESP8266 code for controlling Lamp Relay and several other gadgets connected to ESP8266

More information about the project:

- http://georgik.sinusgear.com/category/iot/

# Related project

- LampAndroidApp - https://github.com/georgik/LampAndroidApp - application which allows remot controll

# Features

## Air quality sensor

Read value from A0 and send it to server.

## MQTT Client

Connects to MQTT broker - send status and receive commands.

## Over the air update - OTA

Push new version to device over WiFi.

## DHT sensor

Meassure temperature and humidity.

Schema: http://www.esp8266learning.com/wemos-mini-pir-sensor-example.php

## PIR sensor

Detect motion and change state of on-board led.

## Relay control

Receive MQTT command and change state of relay.

## RGB LED

RGB LED indicates detection of motion based on PIR.
If no motion was detected then it counts down and display further colors.

Schema: http://www.esp8266learning.com/wemos-rgb-led-example.php

## Web server

Simple web interface with GET API for controlling relay module.

## WiFiManager

Start Config Portal when device is not able to connect to WiFi.
Access 192.168.4.1 IP to configure the device.

#Version 0.2

Configuration is stored in config.json and managed by WiFiManager.

# Version 0.1

Direct communication based on GET API, MQTT and OTA support.
See branch v0.1.

# Customize application

Setup values for MQTT server via WiFiManager
