
Task* waterLevelSensorTask;
int waterLevelSensorPin = D2;
bool lastWaterLevelSensorValue = false;
bool isWaterLevelSensorEnabled = false;

void setupWaterLevelSensor(bool isEnabled, int pin) {
  isWaterLevelSensorEnabled = isEnabled;
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
    const char* message;
    if (sensorValue) {
      message = "high";
    } else {
      message = "low";
    }
    Serial.print("Water Level Sensor: ");
    Serial.println(message);
    sendMessage("water", message);
  }
}

bool getWaterLevelSensor() {
  if (!isWaterLevelSensorEnabled) {
    return false;
  }

  return digitalRead(waterLevelSensorPin);
}
