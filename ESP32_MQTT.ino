/*
 * POC MQTT :
 * ON OFF message with LED topic
 * 
 * support ESP32
 * 
 * by PHM 
 * 
 * @2020
 */
#include <WiFi.h>
#include <PubSubClient.h>

#include "ESP32_PERSONNAL.h"
const char* ssid     = SECRET_SSID;      // SSID
const char* password = SECRET_PASSWORD;  // Password
const char* mqtt_server = "mesures.ludiksciences.fr";
const char* mqtt_topic = "LED";
const char* id = "";

WiFiClient espClient;
PubSubClient client(espClient);

#define ONBOARD_LED  2

void setup()
{
  Serial.begin(115200);
  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, LOW);
  WiFi.begin(ssid, password);
  id = WiFi.macAddress().c_str();
  Serial.println("connected to Wifi");
  client.setServer(mqtt_server, 1883);//connecting to mqtt server
  client.setCallback(callback);
  connectmqtt();
}

void callback(char* topic, byte* payload, unsigned int length) {   //callback includes topic and payload ( from which (topic) the payload is comming)
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  if ((char)payload[0] == 'O' && (char)payload[1] == 'N') //on
  {
    digitalWrite(ONBOARD_LED, HIGH);
    Serial.println("on");
//    client.publish("outTopic", "LED turned ON");
  }
  else if ((char)payload[0] == 'O' && (char)payload[1] == 'F' && (char)payload[2] == 'F') //off
  {
    digitalWrite(ONBOARD_LED, LOW);
    Serial.println(" off");
//    client.publish("outTopic", "LED turned OFF");
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(id)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}


void connectmqtt()
{
  client.connect(id);  // ESP will connect to mqtt broker
  {
    Serial.println("connected to MQTT");
    client.subscribe(mqtt_topic);
    if (!client.connected())
    {
      reconnect();
    }
  }
}
