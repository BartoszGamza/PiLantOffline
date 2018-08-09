#include <LiquidCrystal.h>


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int WaterButtonPin = 7;
const int RefillButtonPin = 6;
const int relay =  8; 
int WaterButtonState = 0;
int RefillButtonState = 0;
int containerState = 4;
unsigned long time_now = 0;
long period = 5000;
//long days = 345600000;

void setup() {
  lcd.begin(16, 2);

  pinMode(relay, OUTPUT);
  pinMode(WaterButtonPin, INPUT);
  pinMode(RefillButtonPin, INPUT);
  digitalWrite(relay, HIGH);
}

int MaxDry = 1023;
int MaxWet = 300;
int PercentMax = 100;
int PercentMin = 0;

void loop() {
  WaterButtonState = digitalRead(WaterButtonPin);
  RefillButtonState = digitalRead(RefillButtonPin);

  if (WaterButtonState == HIGH) {
    if (containerState > 0) {
      Water();
    } else {
      lcd.clear();
      lcd.print("REFILL NEEDED");
      delay(5000);
      lcd.clear();
      ReadSensors();
    }
  } else {
    ReadSensors();
  }
  if (RefillButtonState == HIGH) {
    lcd.clear();
    containerState = 4;
    lcd.print("REFILL SAVED");
    delay(5000);
    lcd.clear();
    ReadSensors();
  } else {
    ReadSensors();
  }
}

void ReadSensors() {
  lcd.setCursor(0, 0);
  lcd.print("container: ");
  lcd.setCursor(11, 0);
  lcd.print(containerState);
  lcd.setCursor(0, 1);
  lcd.print("moist: ");
  
  if(millis() > time_now + period) {
     time_now = millis();
     int moist = analogRead(A0);
     moist = map(moist, MaxDry, MaxWet, PercentMin, PercentMax);
     lcd.setCursor(7, 1);
     lcd.print(moist);
     if (moist < 20 && containerState > 0) {
      Water();
     }
  }
}

void Water() {
  digitalWrite(relay, LOW);
  lcd.clear();
  lcd.print("WATERING");
  delay(70000);
  digitalWrite(relay, HIGH);
  lcd.clear();
  containerState--;
}

