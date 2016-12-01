#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define topicName(a) _topicName((char*)a)
static char* _topicName(char *name)
{
  static char buff[128];
  sprintf(buff, "/%s/%s/%s", getMqttParentTopic(), getHostname(), name);
  return buff;
}

WiFiClient espClient;
PubSubClient mqttClient(espClient);


static void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] (");
  Serial.print(length);
  Serial.print(") ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // commands: on, off, toggle
  if (length > 1) {
    if ((char)payload[0] == 'o') {
      if ((char)payload[1]== 'n') {
        setRelay(HIGH);
      } else {
        setRelay(LOW);
      }
    }
    else if ((char)payload[0] == 't') {
      if (getRelay() == HIGH) {
        setRelay(LOW);
      } else {
        setRelay(HIGH);
      }
    }
  }
}

void reconnect() {

  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (mqttClient.connect(getHostname())) {
      Serial.println("connected");
      mqttClient.publish(topicName("status"), "online");
      mqttClient.subscribe(topicName("command"));
    } else {
      Serial.println("failed");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
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
