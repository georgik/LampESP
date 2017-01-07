#include "DHT.h"

Task* dhtTask;
DHT *dht;
bool isFahrenheit = false;
float lastHumidity = 0;
float lastTemperature = 0;
float lastHeat = 0;
int dhtFailCounter = 0;
float temperatureCorrection = 0;

void logDHTMeasurement() {
  float h = dht->readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht->readTemperature(isFahrenheit) + temperatureCorrection;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    dhtFailCounter++;
    if (dhtFailCounter == 3) {
      dhtTask->disable();
      dhtFailCounter = 0;
    }
    return;
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
  if (!isEnabled) {
    return;
  }
  
  temperatureCorrection = getTemperatureCorrection();

  dht = new DHT(pin, DHT22);
  dht->begin();

  // Meassure once per minute. It seems that 5 second measurement is heating up
  // DHT module about 0.1C per 20 seconds and could reach about 2C higher
  // temperature than should be meassured
  dhtTask = new Task(60000, TASK_FOREVER, &logDHTMeasurement);
  runner.addTask(*dhtTask);
  dhtTask->enable();
}

float getTemperature() {
  return lastTemperature;
}

float getHumidity() {
  return lastHumidity;
}

void handleDHT() {

}
