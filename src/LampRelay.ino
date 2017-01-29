
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
      mqttClient.publish(topicName("relay"), "off", true);
  } else {
    mqttClient.publish(topicName("relay"), "on", true);
  }
}

int getRelay() {
  return digitalRead(relayPin);
}

void handleRelayOn() {
  setRelay(HIGH);
  handleRoot();
}

void handleRelayOff() {
  setRelay(LOW);
  handleRoot();
}
