
int photocellPin = A0;
int photocellThreshold = 220;

void setupPhotocell(bool isEnabled, int pin) {
  if (!isEnabled) {
    return;
  }

  photocellPin = pin;

  photocellThreshold = getPhotocellThreshold();
}

unsigned int getPhotocellValue() {
  return analogRead(photocellPin);
}

bool isPhotocellAboveTreshold() {
  return getPhotocellValue() >= photocellThreshold;
}

bool isPhotocellBelowTreshold() {
  return getPhotocellValue() < photocellThreshold;
}
