int redPin = D2;
int greenPin = D5;
int bluePin = D3;

void setupRGBLED() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  turnOffRGBLED();
}

void turnOffRGBLED() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
}

void greenLED() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
}

void orangeLED() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
}

void blueLED() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
}

void redLED() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
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
