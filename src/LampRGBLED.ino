int redPin = D2;
int greenPin = D5;
int bluePin = D3;

bool isRGBLEDEnabled = false;

void setupRGBLED(bool isEnabled) {
  isEnabled = isRGBLEDEnabled;
  if (!isRGBLEDEnabled) {
    return;
  }
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

void setRGBLED(bool redValue, bool greenValue, bool blueValue) {
  if (!isRGBLEDEnabled) {
    return;
  }
  digitalWrite(redPin, redValue);
  digitalWrite(greenPin, greenValue);
  digitalWrite(bluePin, blueValue);
}

void setColor(int state) {
  switch (state) {
    case 5: {
      redLED();
    }
    case 4: {
      orangeLED();
      break;
    }
    case 3: {
      greenLED();
      break;
    }
    case 2: {
      blueLED();
      break;
    }
    case 1: {
      turnOffRGBLED();
      break;
    }
  }
}
