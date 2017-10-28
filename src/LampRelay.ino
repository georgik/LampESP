
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

const char *RELAY_COMMAND = "command";

void handleRelayCommand(String topic, String payload) {
  if (!isRelayEnabled) {
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(payload);
  if (!json.success()) {
    Serial.println("LampRelay: Parsing JSON failed");
    return;
  }

  String command = "";
  if (json.containsKey(RELAY_COMMAND)) {
    command = json[RELAY_COMMAND].asString();
  }

  if (command == "on") {
    setRelay(HIGH);
  } else if (command == "off") {
    setRelay(LOW);
  } else if (command == "toggle") {
    if (getRelay() == HIGH) {
      setRelay(LOW);
    } else {
      setRelay(HIGH);
    }
  }

}
