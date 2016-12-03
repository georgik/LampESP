#include <Arduino.h>
#include <TaskScheduler.h>

Scheduler runner;

void setup() {
  Serial.begin(115200);
  delay(10);
  runner.init();

  setupRelay();
  setupRGBLED();
  setupWifi();
  setupOTA();
  setupWebServer();
  setupMQTT();
  setupPIR();
  setupDHT();
  setupLED();
  // setupAirSensor();
}

void loop() {
  handleOTA();
  checkMQTTConnection();
  handleDHT();
  handleWebServer();
  handleMQTT();

  runner.execute();
}
