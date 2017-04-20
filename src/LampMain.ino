#include <Arduino.h>
#include <TaskScheduler.h>

#include "LampMQTT.h"

Scheduler runner;

void setup() {
  Serial.begin(115200);
  delay(10);
  runner.init();

  setupWifi(60);
  setupOTA();
  setupWebServer();

  // Connect to MQTT using topic model:
  // - MQTT_MODEL_OPEN - simple, permissive model with /parent/host/
  // - MQTT_MODEL_BLUEMIX - compatible with IBM BlueMix
  // - MQTT_MODEL_AZURE - compatible with Microsoft Azure
  setupMQTT(MQTT_MODEL_BLUEMIX);
  //setupMQTT(MQTT_MODEL_AZURE);

  // true - to enable functionality
  setupDisplay(true, D2, D8);
  setupRelay(false, D1);
  setupRGBLED(false, D2, D5, D3);
  setupPhotocell(false, A0);
  setupDHT(false, D4);
  setupLED(false, D6);
  setupPIR(false, D7);

  // true - to enable functionality
  setupAirSensor(false, A0);
  setupWaterLevelSensor(false, D2);

  setupDeepSleep(false);
}

void loop() {
  handleOTA();
  checkMQTTConnection();
  handleDHT();
  handleWebServer();
  handleMQTT();

  runner.execute();
}
