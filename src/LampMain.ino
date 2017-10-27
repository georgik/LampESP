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

  // display module
  //setupDisplay(true, D2, D8);

  // relay module
  setupRelay(true, D1);

  setupButton(true, D3);

  //setupRGBLED(false, D2, D5, D3);

  // photocell module
  /*
  setupPhotocell(true, A0);
  setupLED(true, D6);
  setupPIR(true, D7);
  */

  setupDHT(true, D4);
  /*
  // true - to enable functionality
  setupAirSensor(false, A0);
  setupWaterLevelSensor(false, D2);
  */

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
