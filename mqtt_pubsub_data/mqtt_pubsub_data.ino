#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino_JSON.h>

const char *ssid = "ssid"; // Enter your Wi-Fi name
const char *password = "passwor";  // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic_sub = "arkatama/msib/my-iot1/actuator";
const char *topic_pub = "arkatama/msib/my-iot1/sensor";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

unsigned long lastTime = 0;
unsigned long intervalTime = 10000;

const int ledPin = 2;

WiFiClient espClient;
PubSubClient client(espClient);

void connectToMqttBroker(){
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.println("Connecting to Mqtt Broker...");
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("Public EMQX MQTT broker connected");
    } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  String sMsg;
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    sMsg += (char) payload[i];
  }
  Serial.println(sMsg);
  Serial.println("-----------------------");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the Wi-Fi network");
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  connectToMqttBroker();

  if (client.subscribe(topic_sub)) {
    Serial.printf("ESP32 subscribed to topic: %s", topic_sub);
  }
  else {
    Serial.println("Failed to subscribe!!!");
  }
}

void loop() {
  client.loop();
  if ((millis() - lastTime) > intervalTime) {
    if (!client.connected()) {
      connectToMqttBroker();
    }
    int device_id = 1;
    int temperature = 27;
    JSONVar dataObject;
    dataObject["device_id"] = (int) device_id;
    dataObject["data"] = (int) temperature;
    String sData = JSON.stringify(dataObject);
    client.publish(topic_pub, sData.c_str());

    lastTime = millis();
  }  
}
