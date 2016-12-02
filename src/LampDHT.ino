#include "DHT.h"

#define DHTPIN D4

Task* dhtTask;
DHT *dht;
bool isFahrenheit = false;
float lastHumidity = 0;
float lastTemperature = 0;
float lastHeat = 0;

void logDHTMeasurement() {
  float h = dht->readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht->readTemperature(isFahrenheit);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht->computeHeatIndex(t, h, isFahrenheit);

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

  if (lastHeat != hic) {
    lastHeat = hic;
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.println(" *C");
    sendMessage("heat", hic);
  }
}

void setupDHT() {
  dht = new DHT(D4, DHT22);
  dht->begin();

  dhtTask = new Task(5000, TASK_FOREVER, &logDHTMeasurement);
  runner.addTask(*dhtTask);
  dhtTask->enable();
}

void handleDHT() {

}
