#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <SD.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>

#define MQTT_HOST "HOST_NAME_HERE"

const char* getHostname() {
  return "ROOM_NAME_HERE";
}

#define topicName(a) _topicName((char*)a)
static char* _topicName(char *name)
{
  static char buff[128];
  sprintf(buff, "/home/%s/%s", getHostname(), name);
  return buff;
}

const int relayPin = D1;
const int chipSelect = D8;
bool isCard = false;
File myFile;

WiFiManager wifiManager;
WiFiClient espClient;
PubSubClient mqttClient(espClient);

ESP8266WebServer server(80);

void setupOTA() {
    ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.setHostname(getHostname());
  ArduinoOTA.begin();
}

static void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] (");
  Serial.print(length);
  Serial.print(") ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (length == 2) {
    if (((char)payload[0] == 'o') && ((char)payload[1]== 'n')) {
      setRelay(HIGH);
    } else {
      setRelay(LOW);
    }
  } else {
    setRelay(LOW);
  }
}

void reconnect() {

  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (mqttClient.connect(getHostname())) {
      Serial.println("connected");
      mqttClient.publish(topicName("status"), "online");
      mqttClient.subscribe(topicName("command"));
    } else {
      Serial.println("failed");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(relayPin, OUTPUT);

  // Connect to WiFi network
  Serial.println();

  wifiManager.setConfigPortalTimeout(180);
  wifiManager.autoConnect("AutoConnectAP", "password");

  setupOTA();
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");


  server.on("/", handleRoot);
  server.on("/relay=on", handleRelayOn);
  server.on("/relay=off", handleRelayOff);
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();

  mqttClient.setServer(MQTT_HOST, 1883);
  mqttClient.setCallback(callback);


}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setRelay(int state) {
  digitalWrite(relayPin, state);
  if (state == LOW) {
      mqttClient.publish(topicName("relay"), "off", true);
  } else {
    mqttClient.publish(topicName("relay"), "on", true);
  }
}

int getRelay() {
  return digitalRead(relayPin);
}

void handleRelayOn() {
  setRelay(HIGH);
  handleRoot();
}

void handleRelayOff() {
  setRelay(LOW);
  handleRoot();
}

void handleRoot() {
  String message = "<!DOCTYPE html>";
  message += "<html lang='en'>";
  message += "<link href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u\" crossorigin=\"anonymous\">";

  message += "<h2>Lamp is now: ";

  int relayState = getRelay();
  if(relayState == HIGH) {
    message += "On";
  } else {
    message += "Off";
  }
  message += "</h2>";
  message += "<br><br>";

  if(relayState == HIGH) {
    message += "<a class=\"btn btn-secondary btn-lg active\" href=\"/relay=off\">Relay OFF</a>";
  } else {
    message += "<a class=\"btn btn-primary btn-lg active\" href=\"/relay=on\">Relay ON</a> ";
  }
  message += "</html>";

  mqttClient.publish(topicName("status"), "response");
  server.send(200, "text/html", message);
}

void loop() {
  ArduinoOTA.handle();
  if (!mqttClient.connected()) {
    reconnect();
  }

  server.handleClient();
  mqttClient.loop();
}
