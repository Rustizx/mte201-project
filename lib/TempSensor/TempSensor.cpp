/* 
    MTE 201 Measuring Device Project - TempSensor
      Version 1.00
    March, 20th 2023 
*/

#include <TempSensor.h>

TempSensor::TempSensor(int type, int pin, int inputVoltage, int weight) 
{
  ResistanceFilter = new Smoothed<double>();
  ResistanceFilter->begin(SMOOTHED_EXPONENTIAL, weight);

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
  
  ResistanceFilter->add(unknownResistance);
}

double TempSensor::getRawResistance()
{
  return unknownResistance;
}

double TempSensor::getResistance()
{
  return ResistanceFilter->get();
}

double TempSensor::getTemperature()
{
  double currentResistance = unknownResistance;
  double logR = log(currentResistance);

  if (type == THERMISTOR_100K) {
    return -22.58*log(currentResistance) + 285.15;
  } else if (type == THERMISTOR_1K) {
    double f = 1 / (C1_1K + C2_1K * logR + C3_1K * pow(logR, 3));
    return f - 273.15;
  } else if (type == RESISTOR_100K) {
    return 0;
  } else if (type == RESISTOR_1M) {
    return -36.67 * logR + 496.82;
  } else { 
    return 0;
  }
}

