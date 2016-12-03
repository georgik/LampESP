
int pirPin = D7;

bool lastPIRState = LOW;
bool PIRState = LOW;
Task* pirTask;
int pirCountdown = 0;

void setupPIR() {
  pirTask = new Task(1000, TASK_FOREVER, &handlePIR);
  runner.addTask(*pirTask);
  pirTask->enable();
}

bool isPIRHigh() {
  return (digitalRead(pirPin) == HIGH);
}

void handlePIR() {
  PIRState = digitalRead(pirPin);
  if (PIRState != lastPIRState) {
    lastPIRState = PIRState;
    if (PIRState == HIGH) {
      Serial.println("PIR HIGH");
      redLED();
      pirCountdown = 6;
      sendMessage("pir", "high");
    } else {
      Serial.println("PIR LOW");
      greenLED();
      sendMessage("pir", "low");
    }
  }

  if ((pirCountdown > 0) && (PIRState == LOW)) {
    pirCountdown--;
    setColor(pirCountdown);
  }
}
