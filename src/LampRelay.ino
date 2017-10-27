
int relayPin = D1;
bool isRelayEnabled = false;

void setupRelay(bool isEnabled, int pin) {
  isRelayEnabled = isEnabled;
  if (!isRelayEnabled) {
    return;
  }
  relayPin = pin;
  pinMode(relayPin, OUTPUT);
}

void setRelay(int state) {
  if (!isRelayEnabled) {
    return;
  }

  digitalWrite(relayPin, state);
  if (state == LOW) {
    sendMessage("relay", "off");
  } else {
    sendMessage("relay", "on");
  }
}

int getRelay() {
  return digitalRead(relayPin);
}

void toggleRelay() {
  if (!isRelayEnabled) {
    return;
  }
  setRelay(!getRelay());
}

String getRelayCommand() {
  if (isRelayEnabled) {
    return "relay";
  }
  return "";
}

void handleRelayOn() {
  setRelay(HIGH);
  handleRoot();
}

void handleRelayOff() {
  setRelay(LOW);
  handleRoot();
}
