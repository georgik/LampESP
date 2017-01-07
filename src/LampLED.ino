
int ledPin = D6;

void setupLED(bool isEnabled, int pin) {
  if (!isEnabled) {
    return;
  }

  ledPin = pin;
  
  pinMode(ledPin, OUTPUT);
  turnOffLED();
}

void turnOffLED() {
  digitalWrite(ledPin, LOW);
}

void turnOnLED() {
  digitalWrite(ledPin, HIGH);
}
