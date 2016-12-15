
int photocellPin = A0;
int photocellTreshold = 220;

void setupPhotocell() {
}

unsigned int getPhotocellValue() {
  return analogRead(photocellPin);
}

bool isPhotocellAboveTreshold() {
  return getPhotocellValue() >= photocellTreshold;
}

bool isPhotocellBelowTreshold() {
  return getPhotocellValue() < photocellTreshold;
}
