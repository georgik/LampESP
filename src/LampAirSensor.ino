
int airSensorPin = A0;
int sensorValue = 0;
int lastSensorValue = 0;
Task* airSensorTask;

void setupAirSensor(bool isEnabled) {
  if (!isEnabled) {
    return;
  }
  airSensorTask = new Task(60000, TASK_FOREVER, &handleAirSensor);
  runner.addTask(*airSensorTask);
  airSensorTask->enable();
}

void handleAirSensor() {
  sensorValue = analogRead(airSensorPin);
  if (sensorValue != lastSensorValue) {
    lastSensorValue = sensorValue;
    Serial.print("Air Sensor: ");
    Serial.println(sensorValue);
    sendMessage("air", sensorValue);
  }
}
