#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "TempSensor.h"

// Vars
const double inputVoltage = 3300;

double temp;

int screen = 0;
bool flip = false;

// Inputs
#define BUTTON_PIN 0
#define RESISTOR3_PIN 36
#define THERM_PIN 33

// OLED Screen 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// TempSensors
TempSensor Resistor1M(RESISTOR_1M, RESISTOR3_PIN, inputVoltage);
TempSensor Thermistor(THERMISTOR_1K, THERM_PIN, inputVoltage);

// Functions
void updateDisplay();
void updateCSV();
void switchScreen();

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);

  // OLED Screen Setup
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(F("time, 1m, thermistor, 1M temp (c), thermistor temp (c)"));
}

void loop() {
  Resistor1M.update();
  Thermistor.update();

  switchScreen();
  updateDisplay();
  updateCSV();
  delay(1000);
}

void switchScreen() {
  if(digitalRead(BUTTON_PIN) == LOW && !flip) {
    if (screen == 0) screen = 1;
    else screen = 0;
    flip = true;
  } else if (digitalRead(BUTTON_PIN) == HIGH && flip) {
    flip = false;
  }
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  if (screen == 0)
  {
    display.println("TEMP");
    display.setTextSize(1);
    display.print("1M Temp    : ");
    display.print(Resistor1M.getTemperature());
    display.println("C");
    display.print("Actual Temp: ");
    display.print(Thermistor.getTemperature());
    display.println("C");
  }
  else
  {
    display.println("RESISTANCE");
    display.setTextSize(1);
    display.print("1M   : ");
    display.print(Resistor1M.getResistance()/1000);
    display.println("K");
    display.print("Therm: ");
    display.println(Thermistor.getResistance());
  }

  display.display();
}

void updateCSV() {
  Serial.print(int(millis()));
  Serial.print(",");
  Serial.print(Resistor1M.getResistance());
  Serial.print(",");
  Serial.print(Thermistor.getResistance());
  Serial.print(",");
  Serial.print(Resistor1M.getTemperature());
  Serial.print(",");
  Serial.println(Thermistor.getTemperature());
}