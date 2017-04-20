#include <ArduinoJson.h>
#include "SSD1306Spi.h"
#include "OLEDDisplayUi.h"

Task* displayTask;
bool isDisplayEnabled = false;
String displayText[] = {"----", "", "", "", "", "", "", "", "", ""};
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

typedef bool (*DisplayPage)(int);

int pageMode = 0;

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
  display->drawStringMaxWidth(0, 0, 128, "----" );
  // Drop contrast to make text little bit more readable
  display->setContrast(10);
  display->display();

  displayTask = new Task(3000, TASK_FOREVER, &handleDisplay);
  runner.addTask(*displayTask);
  displayTask->enable();
}

const char *DISPLAY_OPTION_PAGE = "page";
const char *DISPLAY_OPTION_TEXT = "text";

void handleDisplayCommand(String topic, String payload) {
  if (!isDisplayEnabled) {
    return;
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(payload);
  if (!json.success()) {
    Serial.println("LampDisplay: Parsing JSON failed");
    return;
  }

  int pageIndex = 0;
  if (json.containsKey(DISPLAY_OPTION_PAGE)) {
    pageIndex = json[DISPLAY_OPTION_PAGE].as<int>();
  }

  if (json.containsKey(DISPLAY_OPTION_TEXT)) {
    displayText[pageIndex] = json[DISPLAY_OPTION_TEXT].asString();
  }
}

bool drawText(int pageIndex) {
  String content = displayText[pageIndex];
  int dataLength = content.length();
  // Skip page if content is empty
  if (dataLength == 0) {
    return false;
  }

  if (dataLength < 5) {
    // Font (width x height): 31x62
    display->setFont(DejaVu_LGC_Sans_Mono_52);
  } else if (dataLength < 22) {
    // Font: 24x28
    display->setFont(ArialMT_Plain_24);
  } else if (dataLength < 45) {
    // Font: 16x19
    display->setFont(ArialMT_Plain_16);
  } else {
    // Font: 10x13
    display->setFont(ArialMT_Plain_10);
  }

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawStringMaxWidth(0, 0, 128, content );

  // Render the page
  return true;
}

bool drawConnectionStatus(int pageIndex) {
  if (getMqttFailedConnectionCounter() == 0) {
    // Skip the page
    return false;
  }
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);

  String message = "Connecting to: " + String(getMqttHost());
  message += "; Attempt #" + String(getMqttFailedConnectionCounter());
  display->drawStringMaxWidth(0, 0, 128, message);

  // Render the page
  return true;
}

DisplayPage displayPages[] = {drawText, drawText, drawText, drawText, drawText,
  drawText, drawText, drawText, drawText, drawText, drawConnectionStatus};
int pagesLength = (sizeof(displayPages) / sizeof(DisplayPage));
long timeSinceLastModeSwitch = 0;

String getDisplayCommand() {
  if (isDisplayEnabled) {
    return "display";
  }
  return "";
}

void handleDisplay() {
  display->clear();

  // Render only pages which has something to render
  do {
    pageMode = (pageMode + 1)  % pagesLength;
  } while (displayPages[pageMode](pageMode) == false);

  display->display();
}
