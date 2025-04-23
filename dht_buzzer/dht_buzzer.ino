#include <DHT.h>

const int dhtPin = 26;
const int dhtType = DHT11;

const int buzzerPin = 25;

const float tempThreshold = 30.0;
const float humidityThreshold = 52.0;
const bool useFahrenheit = false;

float humidity;
float temp;

DHT dht(dhtPin, dhtType);

void setup() {
  Serial.begin(115200);
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

  if (temp > tempThreshold || humidity > humidityThreshold) {
    digitalWrite(buzzerPin, HIGH);
    Serial.println("Alarm: Suhu atau kelembaban melebihi batas!");
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  delay(2500);
}
