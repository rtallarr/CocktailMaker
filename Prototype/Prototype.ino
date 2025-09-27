// OLED diplay
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Sonar sensor
#include <Ultrasonic.h>

// PINS
#define relayPin1 2
#define relayPin2 3
#define relayPin3 4
#define relayPin4 5
#define sonarTrig 8
#define sonarEcho 9
#define switchButton 11
#define ledPin 12

const byte relayPins[] = {relayPin1, relayPin2, relayPin3, relayPin4};

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//Trig (emitter) and Echo (receiver)
Ultrasonic ultrasonic(sonarTrig, sonarEcho);

// Relays are active low
const byte relayOn = LOW;
const byte relayOff = HIGH;

// State variables
int switchStatus = 1;
int distance = 0;
int prevDistance = 0;

unsigned long int currentTime;
unsigned long int lastButtonPressed = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Program starting...");
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
  }
  delay(1000);
  display.clearDisplay();

  //Because the Arduino pin is LOW during bootup, the relay could activate for a few milliseconds during that time.
  digitalWrite(relayPin1, relayOff);
  digitalWrite(relayPin2, relayOff);
  digitalWrite(relayPin3, relayOff);
  digitalWrite(relayPin4, relayOff);

  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);

  pinMode(switchButton, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);

  display.setTextColor(WHITE);
  display.setTextSize(1);
  //display.setCursor(0, 8);
  //display.print("Distance");
  //display.drawRect(16, 16, 50, 30, WHITE);
  //display.setCursor(0, 0);
  //display.print("R1");
  //display.setCursor(30, 0);
  //display.print("R2");
  //display.setCursor(60, 0);
  //display.print("R3");
  //display.setCursor(90, 0);
  //display.print("R4");
  //display.drawLine(0, 15, 128, 15, WHITE);
  //display.setCursor(10, 20);
  //display.print("ON");
  display.display();
}

void loop() {
  currentTime = millis();
  distance = ultrasonic.read();

  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == '0') {
      relaysOff();
    } else if (ch == '1') {
      relaysOn();
    } else if (ch == 't') {
      testPumps();
    } else if (ch == '.') {
      delay(5000);
      digitalWrite(relayPin4, relayOn);
      delay(5000);
      digitalWrite(relayPin4, relayOff);
    }
  }

  if (currentTime % 1000 <= 50) {
    //Serial.print("Uptime: ");
    //Serial.print(currentTime);
    //Serial.println("ms");
    display.fillRect(90, 0, 128, 15, BLACK);
    display.setTextSize(1);
    display.setCursor(90, 0);
    display.println(currentTime/1000);
  }

  if (digitalRead(switchButton) && (currentTime - lastButtonPressed) > 100) {
    lastButtonPressed = currentTime;
    Serial.print("Button pressed: ");
    Serial.println(switchStatus);
    if (!switchStatus) {
      relaysOn();
    } else {
      relaysOff();
    }
    switchStatus = !switchStatus;

    
  }
  
  //Change display only if distance changes
  if (distance != prevDistance) {
    prevDistance = distance;
    //Serial.print("Distance: ");
    //Serial.print(distance);
    //Serial.println("cm");
    display.fillRect(90, 20, 38, 15, BLACK);
    display.setTextSize(2);
    display.setCursor(90, 20);
    display.println(distance);
  }

  display.display();

}

void testPumps() {
  digitalWrite(ledPin, HIGH);
  display.fillRect(0, 0, 50, 64, BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Testing pumps");
  Serial.println("Testing pumps...");
  display.display();
  display.setTextSize(2);
  display.setCursor(5, 20);
  display.print("P");
  for (int i = 0; i < sizeof(relayPins); i++) {
    display.fillRect(20, 20, 10, 20, BLACK);
    display.setCursor(20, 20);
    display.print(i + 1); //Pump number [1-4]
    //Serial.println(relayPins[i]); // Print actual pin number
    digitalWrite(relayPins[i], relayOn);
    display.display();
    delay(2500);
    digitalWrite(relayPins[i], relayOff);
  }
  display.fillRect(0, 0, 80, 64, BLACK);
  digitalWrite(ledPin, LOW);
}

void cleanPumps() {
  Serial.println("Cleaning routine running...");
  digitalWrite(ledPin, HIGH);
  digitalWrite(relayPin1, relayOn);
  digitalWrite(relayPin2, relayOn);
  digitalWrite(relayPin3, relayOn);
  digitalWrite(relayPin4, relayOn);
  delay(1000);
  digitalWrite(relayPin1, relayOff);
  digitalWrite(relayPin2, relayOff);
  digitalWrite(relayPin3, relayOff);
  digitalWrite(relayPin4, relayOff);
  Serial.println("Cleaning routine finished...");
  digitalWrite(ledPin, LOW);
}

void relaysOn() {
  // There are problems with current draw (?)
  Serial.println("Turning on...");
  digitalWrite(relayPin1, relayOn);
  delay(150);
  digitalWrite(relayPin2, relayOn);
  delay(150);
  digitalWrite(relayPin3, relayOn);
  delay(150);
  digitalWrite(relayPin4, relayOn);
  delay(150);
  digitalWrite(LED_BUILTIN, HIGH); 
}

void relaysOff() {
  Serial.println("Turning off...");
  digitalWrite(relayPin1, relayOff);
  digitalWrite(relayPin2, relayOff);
  digitalWrite(relayPin3, relayOff);
  digitalWrite(relayPin4, relayOff);
  digitalWrite(LED_BUILTIN, LOW);
}

// When only connecting one pump with 2 relays
void switchRelays() {
  if (switchStatus == 1) {
    digitalWrite(relayPin1, relayOff);
    digitalWrite(relayPin2, relayOn);
    switchStatus = 2;
  } else {
    digitalWrite(relayPin1, relayOn);
    digitalWrite(relayPin2, relayOff);
    switchStatus = 1;
  }
}