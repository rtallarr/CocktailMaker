const int potPin = A0;
int value = 0;

void setup() {
  pinMode(potPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  value = analogRead(potPin);
  Serial.println(value);
  delay(50);
}
