#include <Arduino.h>
#include <TaskScheduler.h>

Scheduler runner;

void setup() {
  Serial.begin(115200);
  delay(10);
  runner.init();

  setupRelay();
  // true - to enable functionality
  setupRGBLED(false);

  setupWifi(60);
  setupOTA();
  setupWebServer();
  setupMQTT();
  setupDHT();
  setupLED();
  setupPIR();

  // true - to enable functionality
  setupAirSensor(false);
}

void loop() {
  handleOTA();
  checkMQTTConnection();
  handleDHT();
  handleWebServer();
  handleMQTT();

  runner.execute();
}
