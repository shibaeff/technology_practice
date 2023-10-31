#ifndef TEMP
#define TEMP

#include <stdio.h>
#include <cmath>
#include <iostream>


class TempDiff {
public:
    virtual double next(double initial, unsigned long attempts) = 0;
};

class Boltzman : TempDiff {
public:
    double next(double initial, unsigned long attempts);
};

class Cauchy : TempDiff {
public:
    double next(double initial, unsigned long attempts);
};

class Logarithmic : TempDiff {
public:
    double next(double initial, unsigned long attempts);
};

enum class Options {
    WithBoltzman,
    WithCauchy,
    WithLog,
};

class Temperature {
public:
    Temperature(Options withTempRule, double initial);
    void temperatureStep(unsigned long attempts);
    double isBetterEnough(long long delta);
    double getCurrentTemp();

private:
    double initialTemp;
    double currentValue;

    Options tempRule;
    Boltzman boltzman;
    Cauchy cauchy;
    Logarithmic logarithmic;
};



#endif