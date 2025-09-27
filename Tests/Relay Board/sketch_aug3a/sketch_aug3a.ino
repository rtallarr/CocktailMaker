const int relayPin1 = 8;

void setup() {
  pinMode(relayPin1, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == '0') {
      Serial.println("Relay off");
      digitalWrite(relayPin1, LOW);
    } else if (ch == '1') {
      Serial.println("Relay on");
      digitalWrite(relayPin1, HIGH);
    }
  }

  delay(10);
}
