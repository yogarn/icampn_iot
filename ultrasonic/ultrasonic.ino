const int trigPin = 4;
const int echoPin = 19;

float echoTime;
float totalDistance;

float distance;

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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  distance = getDistance();

  Serial.print(distance, 2);
  Serial.println("cm");

  delay(1000);
}
