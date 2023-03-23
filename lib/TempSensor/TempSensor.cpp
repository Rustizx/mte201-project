/* 
    MTE 201 Measuring Device Project - TempSensor
      Version 1.00
    March, 20th 2023 
*/

#include <TempSensor.h>

TempSensor::TempSensor(int type, int pin, int inputVoltage, int weight) 
{
  this->type = type;
  this->pin = pin;
  this->inputVoltage = inputVoltage;

  if (type == RESISTOR_100K || type == THERMISTOR_100K) this->knownResistance = 100000;
  else if (type == THERMISTOR_1K) this->knownResistance = 1000;
  else this->knownResistance = 1000000;
}

void TempSensor::update()
{
  resistorVoltage = analogRead(pin) * inputVoltage / 4096;
  unknownResistance = knownResistance * ((inputVoltage/resistorVoltage) - 1);
}

double TempSensor::getResistance()
{
  return unknownResistance;
}

double TempSensor::getTemperature()
{
  double currentResistance = unknownResistance;
  double logR = log(currentResistance);

  if (type == THERMISTOR_100K) 
  {
    return -22.58 * log(currentResistance) + 285.15;
  } 
  else if (type == THERMISTOR_1K) 
  {
    return 1 / (C1_1K + C2_1K * logR + C3_1K * pow(logR, 3)) - 273.15;
  }
  else if (type == RESISTOR_1M) 
  {
    return -9e-5 * currentResistance + 65.018;
  }
  return 0;
}

