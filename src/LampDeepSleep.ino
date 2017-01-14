
Task* deepSleepTask;
unsigned long deepSleepInterval = 0;

void setupDeepSleep(bool isEnabled) {
  if (!isEnabled) {
    return;
  }

  // Check sleep status every 60 seconds
  deepSleepTask = new Task(60000, TASK_FOREVER, &handleDeepSleep);
  runner.addTask(*deepSleepTask);
}

void handleDeepSleep() {
  if (deepSleepInterval > 0) {
    Serial.print("Entering deep sleep for ");
    Serial.println(deepSleepInterval);
    ESP.deepSleep(deepSleepInterval * 1000000);
  }
}
