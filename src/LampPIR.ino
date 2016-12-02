
int pirPin = D7;

bool lastPIRState = LOW;
bool PIRState = LOW;
Task* pirTask;

void setupPIR() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH );

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
      digitalWrite(BUILTIN_LED, LOW );
      sendMessage("pir", "low");
    } else {
      Serial.println("PIR LOW");
      digitalWrite(BUILTIN_LED, HIGH );
      sendMessage("pir", "high");
    }
  }
}
