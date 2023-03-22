#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "TempSensor.h"

// Vars
const double inputVoltage = 3300;

double temp;

int state = 0;
bool flip = false;

// Inputs
#define BUTTON_PIN 0
#define TEMP_PIN 25
#define RESISTOR2_PIN 34
#define RESISTOR3_PIN 36
#define THERM_PIN 33

// OLED Screen 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// TempSensors
TempSensor Resistor100K(RESISTOR_100K, RESISTOR2_PIN, inputVoltage);
TempSensor Resistor1M(RESISTOR_1M, RESISTOR3_PIN, inputVoltage);
TempSensor Thermistor(THERMISTOR_1K, THERM_PIN, inputVoltage);

// Functions
void updateDisplay();
void updateCSV();
void checkButton();

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
  Serial.println(F("state, time, 100k, 100k unfiltered, 1m, 1m unfiltered, thermistor, thermistor unfiltered, 1M temp (c), thermistor temp (c)"));
}

void loop() {
  Resistor100K.update();
  Resistor1M.update();
  Thermistor.update();

  checkButton();
  updateDisplay();
  updateCSV();
  delay(250);
}

void checkButton() {
  if(digitalRead(BUTTON_PIN) == LOW && !flip) {
    state++;
    flip = true;
  } else if (digitalRead(BUTTON_PIN) == HIGH && flip) {
    flip = false;
  }
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println("TEMP");
  display.setTextSize(1);
  display.print("100K Res: ");
  display.print(int(Resistor100K.getResistance()/1000));
  display.println("K");
  display.print("1M Res: ");
  display.print(int(Resistor1M.getResistance()/1000));
  display.println("K");
  display.print("Thermistor: ");
  display.println(int(Thermistor.getResistance()));
  display.print("Therm Temp: ");
  display.print(int(Thermistor.getTemperature()));
  display.println("C");
  display.print("1M Temp: ");
  display.print(int(Resistor1M.getTemperature()));
  display.println("C");
  display.display();
}

void updateCSV() {
  Serial.print(state);
  Serial.print(",");
  Serial.print(int(millis()));
  Serial.print(",");
  Serial.print(Resistor100K.getResistance());
  Serial.print(",");
  Serial.print(Resistor100K.getRawResistance());
  Serial.print(",");
  Serial.print(Resistor1M.getResistance());
  Serial.print(",");
  Serial.print(Resistor1M.getRawResistance());
  Serial.print(",");
  Serial.print(Thermistor.getResistance());
  Serial.print(",");
  Serial.print(Thermistor.getRawResistance());
  Serial.print(",");
  Serial.print(Resistor1M.getTemperature());
  Serial.print(",");
  Serial.println(Thermistor.getTemperature());
}