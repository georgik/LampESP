#include "DHT.h"

Task* dhtTask;
DHT *dht;
bool isFahrenheit = false;
bool isDHTEnabled = false;
float lastHumidity = 0;
float lastTemperature = 0;
float lastHeat = 0;
int dhtFailCounter = 0;
float temperatureCorrection = 0;

void logDHTMeasurement() {
  // Initialize DHT in case of previous failure
  if (dhtFailCounter > 1) {
    dht->begin();
  }

  float h = dht->readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht->readTemperature(isFahrenheit) + temperatureCorrection;

  // Second attempt to read values using initialization in case of failure
  if (isnan(h) || isnan(t)) {
    dht->begin();
    h = dht->readHumidity();
    t = dht->readTemperature(isFahrenheit) + temperatureCorrection;
  }

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    dhtFailCounter++;
    if (dhtFailCounter == 5) {
      isDHTEnabled = false;
      dhtTask->disable();
      dhtFailCounter = 0;
      sendMessage("temperature", "n/a");
      sendMessage("humidity", "n/a");
    }
    return;
  } else {
    dhtFailCounter = 0;
  }

  // Send value only when it changed
  if (lastTemperature != t) {
    lastTemperature = t;
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");
    sendMessage("temperature", t);
  }

  if (lastHumidity != h) {
    lastHumidity = h;
    Serial.print("Humidity: ");
    Serial.println(h);
    sendMessage("humidity", h);
  }
}

void setupDHT(bool isEnabled, int pin) {
  isDHTEnabled = isEnabled;
  if (!isEnabled) {
    return;
  }

  temperatureCorrection = getTemperatureCorrection();

  dht = new DHT(pin, DHT22);
  dhtFailCounter = 0;
  dht->begin();

  // Meassure once per minute. It seems that 5 second measurement is heating up
  // DHT module about 0.1C per 20 seconds and could reach about 2C higher
  // temperature than should be meassured
  dhtTask = new Task(60000, TASK_FOREVER, &logDHTMeasurement);
  runner.addTask(*dhtTask);
  dhtTask->enable();
}

float getTemperature() {
  if (!isDHTEnabled) {
    return 0;
  }

  return lastTemperature;
}

float getHumidity() {
  if (!isDHTEnabled) {
    return 0;
  }

  return lastHumidity;
}

void handleDHT() {

}
