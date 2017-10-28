#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int mqttModel = MQTT_MODEL_OPEN;

// How many times connection to MQTT failed since last success
unsigned int mqttFailedConnectionCounter = 0;

unsigned int getMqttFailedConnectionCounter() {
  return mqttFailedConnectionCounter;
}

#define topicName(a) _topicName((char*)a)
static char* _topicName(char *name)
{
  static char buff[128];
  sprintf(buff, "/%s/%s/%s", getMqttParentTopic(), getHostname(), name);
  return buff;
}

#define topic(a,b) _topic((char*)a, (char*) b)
static char* _topic(char *group, char *name)
{
  static char buff[128];
  sprintf(buff, "/%s/%s/%s", getMqttParentTopic(), group, name);
  return buff;
}

WiFiClient espClient;
PubSubClient mqttClient(espClient);
bool isDayligh = false;

static void callback(char* topicChar, byte* payloadByte, unsigned int length) {
  String topic = topicChar;

  // Default size is defined in PubSubClient library and it's limited to 128
  // https://github.com/knolleary/pubsubclient
  char buf[MQTT_MAX_PACKET_SIZE];
  if (length >= MQTT_MAX_PACKET_SIZE) {
    length = MQTT_MAX_PACKET_SIZE - 1;
  }
  snprintf(buf, length + 1, "%s", payloadByte);

  String payload = String((char *)buf);

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] (");
  Serial.print(length);
  Serial.print(") ");
  Serial.print(payload);
  Serial.println();

  if (topic.endsWith("daylight")) {
    if (payload == "true") {
      isDayligh = true;
      // turnOffPIR();
      turnOffLED();
    } else {
      isDayligh = false;
      // turnOnPIR();
    }
  } else if (topic.indexOf("relay") != -1) {
    handleRelayCommand(topic, payload);

    handleRGBLEDCommand(payload);

  } else if (topic.endsWith("sleep")) {
    deepSleepInterval = payload.toInt();
  } else if (topic.indexOf("display") != -1) {
    handleDisplayCommand(topic, payload);
  }
}

unsigned long waitForReconnectTime = 0;

void publishStatus(char* json) {
  if (mqttModel == MQTT_MODEL_BLUEMIX) {
    mqttClient.publish("iot-2/evt/status/fmt/json", json);
  }
}

void subscribeCommand(String command) {
  if (command.length() == 0) {
    return;
  }
  if (mqttModel == MQTT_MODEL_BLUEMIX) {
    char commandBuffer[256];
    char buf[256];

    command.toCharArray(commandBuffer, command.length() + 1);
    sprintf(buf, "iot-2/cmd/%s/fmt/json", commandBuffer);
    Serial.print("MQTT Subscription: ");
    Serial.println(buf);
    mqttClient.subscribe(buf);
  }
}

void reconnect() {

  if (waitForReconnectTime < millis()) {
    Serial.print("Attempting MQTT connection...");

    // if (mqttClient.connect(getHostname())) {
    if (mqttClient.connect(getHostname(), getMqttUsername(), getMqttPassword())) {
      Serial.println("connected");
      publishStatus("{\"d\":{\"value\":\"online\"}}");

      // Subscribe only to topics which are relevant to enabled modules
      subscribeCommand(getDisplayCommand());
      subscribeCommand(getRelayCommand());
      subscribeCommand(getLEDCommand());
      subscribeCommand(getRGBLEDCommand());

      //mqttClient.subscribe(topic("info", "daylight"));
      mqttFailedConnectionCounter = 0;
    } else {
      Serial.println("failed");
      mqttFailedConnectionCounter++;
    }
    // Any retry should occure in 5 seconds
    waitForReconnectTime = millis() + 5000;
  }
}

void sendMessage(const char* topic, float value) {
  static char buff[16];
  String(value).toCharArray(buff, 15);
  sendMessage(topic, buff);
}

void sendMessage(const char* topic, const char* value) {
  if(!mqttClient.connected()) {
    return;
  }
  if (mqttModel == MQTT_MODEL_BLUEMIX) {
    static char buff[128];
    sprintf(buff, "iot-2/evt/%s/fmt/text", topic);
    mqttClient.publish(buff, value);
  } else {
    mqttClient.publish(topicName(topic), value);
  }
}

void setupMQTT(int model) {
  mqttModel = model;
  mqttClient.setServer(getMqttHost(), getMqttPort());
  mqttClient.setCallback(callback);
}

void handleMQTT() {
  mqttClient.loop();
}

void checkMQTTConnection() {
  if (!mqttClient.connected()) {
    reconnect();
  }

}
