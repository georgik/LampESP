#include <ArduinoJson.h>
#include <FS.h>
#include <WiFiManager.h>

char configMqttHost[64];
char configMqttPort[6];
char configMqttUsername[16];
char configMqttPassword[32];
char configMqttParentTopic[40];
char configDeviceHostname[40];
char configTemperatureCorrection[6];
char configPhotocellThreshold[6];
char configPIRUpInterval[6];
char configDHTInterval[6];

const char *CFG_MQTT_HOST = "mqtt_host";
const char *CFG_MQTT_PORT = "mqtt_port";
const char *CFG_MQTT_USERNAME = "mqtt_username";
const char *CFG_MQTT_PASSWORD = "mqtt_password";
const char *CFG_MQTT_PARENT_TOPIC = "mqtt_parent_topic";
const char *CFG_HOSTNAME = "hostname";
const char *CFG_TEMPERATURE_CORRECTION = "temperature_correction";
const char *CFG_PHOTOCELL_THRESHOLD = "photocell_threshold";
const char *CFG_PIR_UP_INTERVAL = "pir_up_interval";
// Interval between two measurements
const char *CFG_DHT_INTERVAL = "dht_interval";

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

const char* getMqttUsername() {
  return configMqttUsername;
}

const char* getMqttPassword() {
  return configMqttPassword;
}

float getTemperatureCorrection() {
  return String(configTemperatureCorrection).toFloat();
}

int getPhotocellThreshold() {
  return String(configPhotocellThreshold).toInt();
}

int getPIRUpInterval() {
  return String(configPIRUpInterval).toInt();
}

int getDHTInterval() {
  return String(configDHTInterval).toInt();
}

const char* getMqttPortAsString() {
  return configMqttPort;
}

void setConfigValue(String key, String value) {
  int valueLength = value.length()+1;
  if (key == CFG_MQTT_HOST) {
    value.toCharArray(configMqttHost, valueLength);
  } else if (key == CFG_MQTT_PORT) {
    value.toCharArray(configMqttPort, valueLength);
  } else if (key == CFG_MQTT_USERNAME) {
    value.toCharArray(configMqttUsername, valueLength);
  } else if (key == CFG_MQTT_PASSWORD) {
    value.toCharArray(configMqttPassword, valueLength);
  } else if (key == CFG_MQTT_PARENT_TOPIC) {
    value.toCharArray(configMqttParentTopic, valueLength);
  } else if (key == CFG_HOSTNAME) {
    value.toCharArray(configDeviceHostname, valueLength);
  } else if (key == CFG_TEMPERATURE_CORRECTION) {
    value.toCharArray(configTemperatureCorrection, valueLength);
  } else if (key == CFG_PHOTOCELL_THRESHOLD) {
    value.toCharArray(configPhotocellThreshold, valueLength);
  } else if (key == CFG_PIR_UP_INTERVAL) {
    value.toCharArray(configPIRUpInterval, valueLength);
  } else if (key == CFG_DHT_INTERVAL) {
    value.toCharArray(configDHTInterval, valueLength);
  }
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

          loadConfigValue(json, configMqttHost, CFG_MQTT_HOST);
          loadConfigValue(json, configMqttPort, CFG_MQTT_PORT);
          loadConfigValue(json, configMqttUsername, CFG_MQTT_USERNAME);
          loadConfigValue(json, configMqttPassword, CFG_MQTT_PASSWORD);
          loadConfigValue(json, configMqttParentTopic, CFG_MQTT_PARENT_TOPIC);
          loadConfigValue(json, configDeviceHostname, CFG_HOSTNAME);
          loadConfigValue(json, configTemperatureCorrection, CFG_TEMPERATURE_CORRECTION);
          loadConfigValue(json, configPhotocellThreshold, CFG_PHOTOCELL_THRESHOLD);
          loadConfigValue(json, configPIRUpInterval, CFG_PIR_UP_INTERVAL);
          loadConfigValue(json, configDHTInterval, CFG_DHT_INTERVAL);

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
  WiFiManagerParameter custom_mqtt_host(CFG_MQTT_HOST, "MQTT host", configMqttHost, 40);
  WiFiManagerParameter custom_mqtt_port(CFG_MQTT_PORT, "MQTT port", configMqttPort, 5);
  WiFiManagerParameter custom_mqtt_parent_topic(CFG_MQTT_PARENT_TOPIC, "MQTT Parent topic", configMqttParentTopic, 40);
  WiFiManagerParameter custom_hostname(CFG_HOSTNAME, "Device hostname/Room name", configDeviceHostname, 40);

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
  json[CFG_MQTT_HOST] = getMqttHost();
  json[CFG_MQTT_PORT] = getMqttPortAsString();
  json[CFG_MQTT_USERNAME] = getMqttUsername();
  json[CFG_MQTT_PASSWORD] = getMqttPassword();
  json[CFG_MQTT_PARENT_TOPIC] = getMqttParentTopic();
  json[CFG_HOSTNAME] = getHostname();
  json[CFG_TEMPERATURE_CORRECTION] = String(getTemperatureCorrection());
  json[CFG_PHOTOCELL_THRESHOLD] = String(getPhotocellThreshold());
  json[CFG_PIR_UP_INTERVAL] = String(getPIRUpInterval());
  json[CFG_DHT_INTERVAL] = String(getDHTInterval());

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
