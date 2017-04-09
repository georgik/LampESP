#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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
  } else if (topic.endsWith("command")) {
    if (payload == "on") {
      setRelay(HIGH);
    } else if (payload == "off") {
      setRelay(LOW);
    } else if (payload == "toggle") {
      if (getRelay() == HIGH) {
        setRelay(LOW);
      } else {
        setRelay(HIGH);
      }
    }

    handleRGBLEDCommand(payload);

  } else if (topic.endsWith("sleep")) {
    deepSleepInterval = payload.toInt();
  } else if (topic.indexOf("display") != -1) {
    handleDisplayCommand(topic, payload);
  }
}

unsigned long waitForReconnectTime = 0;

void reconnect() {

  if (waitForReconnectTime < millis()) {
    Serial.print("Attempting MQTT connection...");

    if (mqttClient.connect(getHostname())) {
      Serial.println("connected");
      mqttClient.publish(topicName("status"), "online");
      mqttClient.subscribe(topicName("command"));
      mqttClient.subscribe(topicName("display/+"));
      mqttClient.subscribe(topic("info", "daylight"));
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
  mqttClient.publish(topicName(topic), value);
}

void setupMQTT() {
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
