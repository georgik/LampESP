
int pirPin = D7;

bool lastPIRState = LOW;
bool PIRState = LOW;
Task* pirTask;
int pirCountdown = 0;
int pirUpInterval = 30;

void setupPIR(bool isEnabled, int pin) {
  if (!isEnabled) {
    return;
  }

  pirPin = pin;
  pinMode(pirPin, INPUT);
  pirUpInterval = getPIRUpInterval();
  pirTask = new Task(1000, TASK_FOREVER, &handlePIR);
  runner.addTask(*pirTask);
  turnOnPIR();
}

void turnOnPIR() {
  pirCountdown = 0;
  pirTask->enable();
}

void turnOffPIR() {
  pirTask->disable();
}

bool isPIRHigh() {
  return (digitalRead(pirPin) == HIGH);
}

void handlePIR() {
  PIRState = digitalRead(pirPin);

  if (PIRState != lastPIRState) {
    lastPIRState = PIRState;

    if (PIRState == HIGH) {
      if ((isPhotocellBelowTreshold()) && (!isDayligh)) {
        if (pirCountdown <= 0) {
          turnOnLED();
          sendMessage("pir", "high");
          Serial.println("PIR HIGH");
        }

        // Keep PIR up for defined period of time in seconds
        pirCountdown = pirUpInterval;
      }
    }
  }

  if (pirCountdown > 0) {
    pirCountdown--;
    if (isPhotocellAboveTreshold()) {
      pirCountdown = 0;
    }
  }

  if (pirCountdown == 0) {
    pirCountdown--;
    turnOffLED();
    Serial.println("PIR LOW");
    sendMessage("pir", "low");
  }

}
