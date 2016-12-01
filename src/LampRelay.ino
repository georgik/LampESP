
const int relayPin = D1;

void setupRelay() {
  pinMode(relayPin, OUTPUT);
}

void setRelay(int state) {
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
