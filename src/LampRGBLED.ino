int redPin = D2;
int greenPin = D5;
int bluePin = D3;

bool isRGBLEDEnabled = false;

void setupRGBLED(bool isEnabled, int mRedPin, int mGreenPin, int mBluePin) {
  isRGBLEDEnabled = isEnabled;
  if (!isRGBLEDEnabled) {
    return;
  }

  redPin = mRedPin;
  greenPin = mGreenPin;
  bluePin = mBluePin;

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  turnOffRGBLED();
}

void turnOffRGBLED() {
  setRGBLED(HIGH, HIGH, HIGH);
}

void greenLED() {
  setRGBLED(HIGH, LOW, HIGH);
}

void orangeLED() {
  setRGBLED(LOW, LOW, HIGH);
}

void blueLED() {
  setRGBLED(HIGH, HIGH, LOW);
}

void redLED() {
  setRGBLED(LOW, HIGH, HIGH);
}

void whiteLED() {
  setRGBLED(LOW, LOW, LOW);
}

void setRGBLED(bool redValue, bool greenValue, bool blueValue) {
  if (!isRGBLEDEnabled) {
    return;
  }
  digitalWrite(redPin, redValue);
  digitalWrite(greenPin, greenValue);
  digitalWrite(bluePin, blueValue);
}

void handleRGBLEDCommand(String payload) {
  if (!isRGBLEDEnabled) {
    return;
  }

  if (payload == "green") {
    greenLED();
  } else if (payload == "yellow") {
    orangeLED();
  } else if (payload == "orange") {
    orangeLED();
  } else if (payload == "red") {
    redLED();
  } else if (payload == "blue") {
    blueLED();
  } else if (payload == "white") {
    whiteLED();
  } else if (payload == "black") {
    turnOffRGBLED();
  }
}
