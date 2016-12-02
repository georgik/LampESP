#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(10);

  setupRelay();
  setupPIR();
  setupWifi();
  setupOTA();
  setupWebServer();
  setupMQTT();
}

void loop() {
  handleOTA();
  checkMQTTConnection();
  handlePIR();
  handleWebServer();
  handleMQTT();
}
