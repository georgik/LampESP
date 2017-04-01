#include "SSD1306Spi.h"
#include "OLEDDisplayUi.h"

Task* displayTask;
bool isDisplayEnabled = false;
String displayText = "-=[LampESP]=- georgik.rocks";
int displayDCPin = D2;
int displayCSPin = D8;

// More information: https://github.com/squix78/esp8266-oled-ssd1306/blob/master/examples/SSD1306SimpleDemo/SSD1306SimpleDemo.ino
// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi        display(D0, D2, D8);

SSD1306Spi *display;
// SH1106 display(0x3c, D3, D5);

typedef void (*DisplayPage)(void);

int pageMode = 0;
int counter = 1;

void setupDisplay(bool isEnabled, int DCPin, int CSPin) {

  if (!isEnabled) {
    return;
  }
  isDisplayEnabled = isEnabled;
  displayDCPin = DCPin;
  displayCSPin = CSPin;
  display = new SSD1306Spi(D0, displayDCPin, displayCSPin);

  display->init();
  display->flipScreenVertically();
  display->setFont(ArialMT_Plain_10);
  display->drawStringMaxWidth(0, 0, 128, displayText );
  // Drop contrast to make text little bit more readable
  display->setContrast(10);
  display->display();

  displayTask = new Task(3000, TASK_FOREVER, &handleDisplay);
  runner.addTask(*displayTask);
  displayTask->enable();
}


void handleDisplayCommand(String payload) {
  if (!isDisplayEnabled) {
    return;
  }
  displayText = payload;
}

void drawText() {
  display->setFont(Roboto_Light_54);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawStringMaxWidth(0, 0, 128, displayText );
}


DisplayPage displayPages[] = {drawText};
int pagesLength = (sizeof(displayPages) / sizeof(DisplayPage));
long timeSinceLastModeSwitch = 0;

void handleDisplay() {
  display->clear();
  displayPages[pageMode]();

  display->display();

  pageMode = (pageMode + 1)  % pagesLength;
  counter++;
}
