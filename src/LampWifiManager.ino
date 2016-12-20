#include <ArduinoJson.h>
#include <FS.h>
#include <WiFiManager.h>

char configMqttHost[40];
char configMqttPort[6];
char configMqttParentTopic[40];
char configDeviceHostname[40];
char configTemperatureCorrection[6];
char configPhotocellThreshold[6];

WiFiManager wifiManager;

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

const char* getHostname() {
  return configDeviceHostname;
}

const char* getMqttParentTopic() {
  return configMqttParentTopic;
}

const char* getMqttHost() {
  return configMqttHost;
}

int getMqttPort() {
  return String(configMqttPort).toInt();
}

float getTemperatureCorrection() {
  return String(configTemperatureCorrection).toFloat();
}

int getPhotocellThreshold() {
  return String(configPhotocellThreshold).toInt();
}

const char* getMqttPortAsString() {
  return configMqttPort;
}

void setConfigValue(String key, String value) {
  int valueLength = value.length()+1;
  if (key == "mqtt_host") {
    value.toCharArray(configMqttHost, valueLength);
  } else if (key == "mqtt_port") {
    value.toCharArray(configMqttPort, valueLength);
  } else if (key == "mqtt_parent_topic") {
    value.toCharArray(configMqttParentTopic, valueLength);
  } else if (key == "hostname") {
    value.toCharArray(configDeviceHostname, valueLength);
  } else if (key == "temperature_correction") {
    value.toCharArray(configTemperatureCorrection, valueLength);
  } else if (key == "photocell_threshold") {
    value.toCharArray(configPhotocellThreshold, valueLength);
  }
}

void setDefaultConfig() {
  strcpy(configMqttHost, "iot.sinusgear.com");
  strcpy(configMqttPort, "1883");
  strcpy(configMqttParentTopic, "home");
  strcpy(configDeviceHostname, "esp8266");
  strcpy(configTemperatureCorrection, "0");
  strcpy(configPhotocellThreshold, "220");
}

void printMacAddress() {
  uint8_t MAC_array[6];

  WiFi.macAddress(MAC_array);
  Serial.println("");
  Serial.print("MAC address: ");
  for (int i = 0; i < sizeof(MAC_array); ++i) {
    if (MAC_array[i]<16) {
      Serial.print("0");
    }
    Serial.print(String(MAC_array[i], HEX));
    Serial.print(":");
  }

  Serial.println("");
}

void loadConfigValue(ArduinoJson::JsonObject& json, char* targetVar, String keyName) {
  if (json.containsKey(keyName)) {
    strcpy(targetVar, json[keyName]);
  }
}

void setupWifi(int portalConfigTimeout) {

  printMacAddress();

  WiFi.mode(WIFI_STA); // Force to station mode because if device was switched off while in access point mode it will start up next time in access point mode.

  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          loadConfigValue(json, configMqttHost, "mqtt_host");
          loadConfigValue(json, configMqttPort, "mqtt_port");
          loadConfigValue(json, configMqttParentTopic, "mqtt_parent_topic");
          loadConfigValue(json, configDeviceHostname, "hostname");
          loadConfigValue(json, configTemperatureCorrection, "temperature_correction");
          loadConfigValue(json, configPhotocellThreshold, "photocell_threshold");

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read

  // id/name, placeholder/prompt, default, length
  WiFiManagerParameter custom_mqtt_host("mqtt_host", "MQTT host", configMqttHost, 40);
  WiFiManagerParameter custom_mqtt_port("mqtt_port", "MQTT port", configMqttPort, 5);
  WiFiManagerParameter custom_mqtt_parent_topic("mqtt_parent_topic", "MQTT Parent topic", configMqttParentTopic, 40);
  WiFiManagerParameter custom_hostname("hostname", "Device hostname/Room name", configDeviceHostname, 40);

  wifiManager.addParameter(&custom_mqtt_host);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_parent_topic);
  wifiManager.addParameter(&custom_hostname);

  wifiManager.setConfigPortalTimeout(portalConfigTimeout);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.autoConnect();

  if (shouldSaveConfig) {
    strcpy(configDeviceHostname, custom_hostname.getValue());
    strcpy(configMqttParentTopic, custom_mqtt_parent_topic.getValue());
    strcpy(configMqttHost, custom_mqtt_host.getValue());
    strcpy(configMqttPort, custom_mqtt_port.getValue());

    saveConfig();
  }

  Serial.println("MQTT host: " + String(configMqttHost));
  Serial.println("MQTT port: " + String(configMqttPort));
  Serial.println("MQTT parent topic: " + String(configMqttParentTopic));
  Serial.println("hostname:" + String(configDeviceHostname));
}

void saveConfig() {
  Serial.println("saving config");
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["mqtt_host"] = getMqttHost();
  json["mqtt_port"] = getMqttPortAsString();
  json["mqtt_parent_topic"] = getMqttParentTopic();
  json["hostname"] = getHostname();
  json["temperature_correction"] = String(getTemperatureCorrection());
  json["photocell_threshold"] = String(getPhotocellThreshold());

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
    return;
  }

  json.printTo(Serial);
  json.printTo(configFile);
  configFile.close();
  //end save
}
