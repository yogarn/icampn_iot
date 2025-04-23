const int ldrPin = 34;
const int ledPin = 16;

float voltage;
float pwmValue;
int sensorValue;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  sensorValue = analogRead(ldrPin);
  Serial.print("Sensor value : ");
  Serial.println(sensorValue);

  pwmValue = map(sensorValue, 1000, 4095, 0, 255);
  analogWrite(ledPin, pwmValue);

  delay(100);
}
