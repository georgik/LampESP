
int pirPin = D7;

bool lastPIRState = LOW;
bool PIRState = LOW;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long timeCumulator = 0;
unsigned long PIRAlarmInterval = 1000;

void setupPIR() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH );
}

bool isPIRHigh() {
  return (digitalRead(pirPin) == HIGH);
}

void handlePIR() {
  unsigned long currentMillis = millis();
  timeCumulator += (currentMillis - previousMillis);

  if (timeCumulator >= PIRAlarmInterval) {
    PIRState = digitalRead(pirPin);
    if (PIRState != lastPIRState) {
      lastPIRState = PIRState;
      if (PIRState == HIGH) {
        Serial.println("PIR HIGH");
        digitalWrite(BUILTIN_LED, LOW );
      } else {
        Serial.println("PIR LOW");
        digitalWrite(BUILTIN_LED, HIGH );
      }
    }
    timeCumulator = 0;
  }
  previousMillis = currentMillis;
  // mqttClient.publish(topicName("status"), "online");
}
