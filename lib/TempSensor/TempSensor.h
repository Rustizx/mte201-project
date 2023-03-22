/* 
    MTE 201 Measuring Device Project - TempSensor
      Version 1.00
    March, 20th 2023 
*/

#ifndef ARCHSENSORS_H
#define ARCHSENSORS_H

#include <Arduino.h>
#include <Smoothed.h>

#define THERMISTOR_100K 0
#define RESISTOR_100K 1
#define RESISTOR_1M 2
#define THERMISTOR_1K 3

#define C1_1K 0.0012613826250546462
#define C2_1K 0.0002936442841673821
#define C3_1K -1.5804597893978733e-7

class TempSensor {
  private:
    int type;
    int pin;

    double inputVoltage;
    double knownResistance;

    double resistorVoltage;
    double unknownResistance;

    Smoothed<double> *ResistanceFilter;

  public:
    TempSensor(int type, int pin, int inputVoltage, int weight = 20);

    void update();

    double getRawResistance();
    double getResistance();
    double getTemperature();
};

#endif