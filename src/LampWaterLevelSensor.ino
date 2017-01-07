
Task* waterLevelSensorTask;
int waterLevelSensorPin = D2;
bool lastWaterLevelSensorValue = false;

void setupWaterLevelSensor(bool isEnabled, int pin) {
  if (!isEnabled) {
    return;
  }

  waterLevelSensorPin = pin;
  pinMode(waterLevelSensorPin, INPUT);

  waterLevelSensorTask = new Task(10000, TASK_FOREVER, &handleWaterLevelSensor);
  runner.addTask(*waterLevelSensorTask);
  waterLevelSensorTask->enable();
}

void handleWaterLevelSensor() {
  bool sensorValue = digitalRead(waterLevelSensorPin);
  if (sensorValue != lastWaterLevelSensorValue) {
    lastWaterLevelSensorValue = sensorValue;
    Serial.print("Water Level Sensor: ");
    Serial.println(sensorValue);
    sendMessage("water", sensorValue);
  }
}
