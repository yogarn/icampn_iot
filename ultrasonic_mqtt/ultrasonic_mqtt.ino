#include <WiFi.h>
#include <ArduinoMqttClient.h>

const int trigPin = 4;
const int echoPin = 19;

const int buzzerPin = 16;

const char* ssid = "<SSID>";
const char* password = "<PASSWORD>";

const char* mqttServer = "<MQTT SERVER IP>";
const int mqttPort = 1883;

const char* topicDistance = "esp/distance";

WiFiClient espClient;
MqttClient client(espClient);

float echoTime;
float totalDistance;

float distance;

void setupWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnectMqtt() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESPClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.connectError());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

float getDistance() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);
  totalDistance = echoTime / 58.26;

  return totalDistance;
}

void setup() {
  Serial.begin(115200);
  setupWiFi();

  client.setId("ESPClient");
  client.setUsernamePassword(ssid, password);

  if (!client.connect(mqttServer, mqttPort)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(client.connectError());
    while (1)
      ;
  }

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(buzzerPin, LOW);
}

void loop() {
  if (!client.connected()) {
    reconnectMqtt();
  }
  client.poll();

  distance = getDistance();

  Serial.print(distance, 2);
  Serial.println("cm");

  client.beginMessage(topicDistance);
  client.print(distance);
  client.endMessage();

  if (distance < 20) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  delay(1000);
}
