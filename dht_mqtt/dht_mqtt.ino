#include <DHT.h>
#include <WiFi.h>
#include <ArduinoMqttClient.h>

const int dhtPin = 26;
const int dhtType = DHT11;

const int buzzerPin = 25;

const char* ssid = "<SSID>";
const char* password = "<PASSWORD>";

const char* mqttServer = "<MQTT SERVER IP>";
const int mqttPort = 1883;

const char* topicHumidity = "esp/humidity";
const char* topicTemp = "esp/temperature";

WiFiClient espClient;
MqttClient client(espClient);

const float tempThreshold = 30.0;
const float humidityThreshold = 52.0;
const bool useFahrenheit = false;

float humidity;
float temp;

DHT dht(dhtPin, dhtType);

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

  pinMode(buzzerPin, OUTPUT);
  dht.begin();
}

void loop() {
  humidity = dht.readHumidity();
  temp = dht.readTemperature(useFahrenheit);

  if (isnan(humidity) || isnan(temp)) {
    Serial.println("Gagal membaca data dari sensor!");
    return;
  }

  Serial.print("Temperature : ");
  Serial.print(temp);
  Serial.print(" || Humidity : ");
  Serial.println(humidity);

  client.beginMessage(topicHumidity);
  client.print(humidity);
  client.endMessage();

  client.beginMessage(topicTemp);
  client.print(temp);
  client.endMessage();

  if (temp > tempThreshold || humidity > humidityThreshold) {
    digitalWrite(buzzerPin, HIGH);
    Serial.println("Alarm: Suhu atau kelembaban melebihi batas!");
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  delay(2500);
}
