#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(10);

  setupRelay();
  setupWifi();
  setupOTA();
  setupWebServer();
  setupMQTT();
}

void loop() {
  handleOTA();
  checkMQTTConnection();
  handleWebServer();
  handleMQTT();
}
