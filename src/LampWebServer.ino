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
  server->on("/relay=on", handleRelayOn);
  server->on("/relay=off", handleRelayOff);
  server->on("/inline", [](){
    server->send(200, "text/plain", "this works as well");
  });

  server->onNotFound(handleNotFound);

  server->begin();

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
  server->send(200, "text/html", message);
}

void handleWebServer() {
  server->handleClient();
}
