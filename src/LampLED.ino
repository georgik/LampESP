
int ledPin = D6;
bool isLEDEnabled = false;

void setupLED(bool isEnabled, int pin) {
  if (!isEnabled) {
    return;
  }
  isLEDEnabled = isEnabled;

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

String getLEDCommand() {
  if (isLEDEnabled) {
    return "rgbled";
  }
  return "";
}
