#include <DHT.h>

const int dhtPin = 26;
const int dhtType = DHT11;

DHT dht(dhtPin, dhtType);

const bool useFahrenheit = false;

float humidity;
float temp;

void setup() {
  Serial.begin(115200);
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

  delay(2500);
}
