
int buttonPin = D3;
bool isButtonEnabled = false;
Task* buttonTask;
bool lastButtonState = HIGH;

void setupButton(bool isEnabled, int pin) {
  if (!isEnabled) {
    return;
  }
  isButtonEnabled = isEnabled;

  buttonPin = pin;

  pinMode(buttonPin, INPUT);
  buttonTask = new Task(200, TASK_FOREVER, &handleButton);
  runner.addTask(*buttonTask);
  buttonTask->enable();
}

bool getButtonState() {
  if (!isButtonEnabled) {
    return HIGH;
  }

  return digitalRead(buttonPin);
}

void handleButton() {
  bool buttonState = getButtonState();

  if (buttonState == lastButtonState) {
    return;
  }

  if (lastButtonState == LOW) {
    toggleRelay();
    // debounce interval
    delay(500);
  }

  lastButtonState = buttonState;
}
