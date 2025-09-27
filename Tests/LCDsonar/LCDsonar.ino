#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>

const int button1Pin = 5;
const int button2Pin = 4;

//Trig (emitter) and Echo (receiver)
Ultrasonic ultrasonic(12, 11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int distance = 0;
int prevDistance = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);

  lcd.init();       // initialize the LCD
  lcd.clear();      // clear the LCD display
  lcd.backlight();  // Make sure backlight is on
  lcd.setCursor(1, 0);
  lcd.print("Distance: ");

  Serial.begin(9600);
  Serial.println("1- on\n2- off");
}

void loop() {
  int button1Status = digitalRead(button1Pin);
  int button2Status = digitalRead(button2Pin);

  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == '1') {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Led on");
    } else if (ch == '0') {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Led off");
    } else if (ch == 'x') {
      Serial.println("Turning off...");
      digitalWrite(LED_BUILTIN, LOW);
      lcd.noBacklight();
      lcd.clear();
    }
  }

  distance = ultrasonic.read();
  Serial.println(distance);
  
  //Change lcd display only if distance changes
  if (distance != prevDistance) {
    prevDistance = distance;
    lcd.setCursor(13, 1);
    lcd.print("   ");
    lcd.setCursor(13, 1);
    lcd.print(distance);
    Serial.println("updating distance");
  }

  //Serial.print("Button 1: ");
  //Serial.print(button1Status);
  //Serial.print(" Button 2: ");
  //Serial.print(button2Status);
  //Serial.print(" Potentiometer: ");
  //Serial.println(potValue);

  delay(250);
}
