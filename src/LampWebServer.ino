#include <ESP8266WebServer.h>

ESP8266WebServer *server;

void setupWebServer() {
  server = new ESP8266WebServer(80);

  // Start the server
  server->begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");


  server->on("/", handleRoot);
  server->on("/config", handleConfig);
  server->on("/reboot", handleReboot);
  server->on("/relay=on", handleRelayOn);
  server->on("/relay=off", handleRelayOff);
  server->on("/inline", [](){
    server->send(200, "text/plain", "this works as well");
  });

  server->onNotFound(handleNotFound);

  server->begin();

}

void handleReboot() {
    String message = "Rebooting...";
    server->send(200, "text/html", message);
    ESP.reset();
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i=0; i<server->args(); i++){
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);
}

void handleConfig() {
  String message = "Configuration\n\n";
  for (uint8_t i=0; i<server->args(); i++){
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
    setConfigValue(server->argName(i), server->arg(i));
  }
  saveConfig();
  server->send(200, "text/html", message);
}


void handleRoot() {
  String message = "<!DOCTYPE html>";
  message += "<html lang='en'>";
  message += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>";
  message += "<link href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u\" crossorigin=\"anonymous\">";

  message += "<body><h2><a href='/'>Lamp ESP</a></h2> ";

  int relayState = getRelay();

  message += "<br><br>";

  if(relayState == HIGH) {
    message += "<a class=\"btn btn-secondary btn-lg active\" href=\"/relay=off\">Relay OFF</a>";
  } else {
    message += "<a class=\"btn btn-primary btn-lg active\" href=\"/relay=on\">Relay ON</a> ";
  }

  message += "<h2>Info</h2><ul>";
  message += "<li>Host: " + String(getHostname()) + "</li>";
  message += "<li>Signal strength: " + String(WiFi.RSSI()) + " dB</li>";

  message += "<li>MQTT status: ";
  if (mqttClient.connected()) {
    message += "connected";
  } else {
    message += "disconnected";
  }
  message += "</li>";

  message += "<li>MQTT host: " + String(getMqttHost()) + "</li>";
  message += "<li>PIR Countdown: " + String(pirCountdown) +"s </li>";
  message += "<li>Photocell value: " + String(getPhotocellValue()) + " (threshold: " + String(getPhotocellThreshold())+ ")</li>";
  message += "<li>Temperature: " + String(getTemperature()) + " (applied correction: " + String(getTemperatureCorrection()) + ")</li>";
  message += "<li>Humidity: " + String(getHumidity()) + "</li>";
  message += "<li>Water Level: " + String(getWaterLevelSensor()) + "</li>";

  message += "</ul></body></html>";

  server->send(200, "text/html", message);
}

void handleWebServer() {
  server->handleClient();
}
