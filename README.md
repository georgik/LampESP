# Lamp ESP module

ESP8266 code for controlling Lamp Relay and several other gadgets connected to ESP8266

More information about the project:

- http://georgik.sinusgear.com/category/iot/

# Related project

- LampAndroidApp - https://github.com/georgik/LampAndroidApp - application which allows remot controll

# Features

## MQTT Client

Connect to MQTT broker - send status and receive commands.

## Over the air update - OTA

Push new version to device over WiFi.

## PIR sensor

Detect motion and change state of on-board led.

## Relay control

Receive MQTT command and change state of relay.

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
