
int ledPin = D6;

void setupLED() {
  pinMode(ledPin, OUTPUT);
  turnOffLED();
}

void turnOffLED() {
  digitalWrite(ledPin, LOW);
}

void turnOnLED() {
  digitalWrite(ledPin, HIGH);
}
