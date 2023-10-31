//
// Created by Pavel Shibaev on 30.10.23.
//
#include <random>
#include "Temperature.hpp"


double Boltzman::next(double initial, unsigned long attempts) {
    return initial / std::log(attempts + 1);
}

double Cauchy::next(double initial, unsigned long attempts) {
    return initial / (attempts + 1);
}

double Logarithmic::next(double initial, unsigned long attempts) {
    return initial * std::log(attempts + 1) / (attempts + 1);
}



Temperature::Temperature(Options withTempRule, double initial) :
        tempRule(withTempRule), initialTemp(initial), currentValue(initial) {}

void Temperature::temperatureStep(unsigned long attempts) {
    if (tempRule == Options::WithBoltzman)
        currentValue = boltzman.next(initialTemp, attempts);
    else if (tempRule == Options::WithCauchy)
        currentValue = cauchy.next(initialTemp, attempts);
    else if (tempRule == Options::WithLog)
        currentValue = logarithmic.next(initialTemp, attempts);
}

double Temperature::isBetterEnough(long long delta) {
    std::random_device rd;
    auto gen = std::mt19937(rd()); // Mersenne Twister random number generator
    std::uniform_real_distribution<> dis = std::uniform_real_distribution<>();
    return dis(gen) > std::exp(-delta / currentValue);
}

double Temperature::getCurrentTemp() {
    return currentValue;
}