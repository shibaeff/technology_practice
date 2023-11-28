#include <math.h>

#include "Types.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

std::string to_string(float number, int precision) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << number;
    return stream.str();
}

Composition::Composition(TFunc &_other, TFunc &_another, std::string type_value) :
        other(_other.Copy()), another(_another.Copy()), type(type_value) {}


std::shared_ptr<TFunc> Composition::Copy() {
    return std::make_shared<Composition>(*other, *another, type);
}

double Composition::Derivative(double num) {
    if (type == "+") {
        return (*other).Derivative(num) + (*another).Derivative(num);
    } else if (type == "-") {
        return (*other).Derivative(num) - (*another).Derivative(num);
    } else if (type == "*") {
        return ((*other).Derivative(num) * (*another)(num) + (*another).Derivative(num) * (*other)(num));
    } else if (type == "/") {
        return ((*other).Derivative(num) * (*another)(num) - (*another).Derivative(num) * (*other)(num)) /
               ((*another)(num) * (*another)(num));
    } else {
        throw std::logic_error("Wrong type of operation");
    }
}

double Composition::operator()(double num) {
    if (type == "+") {
        return (*other)(num) + (*another)(num);
    } else if (type == "-") {
        return (*other)(num) - (*another)(num);
    } else if (type == "*") {
        return (*other)(num) * (*another)(num);
    } else if (type == "/") {
        return (*other)(num) / (*another)(num);
    } else {
        throw std::logic_error("Wrong type of operation");
    }
}

std::string Composition::ToString() const {
    if (type == "+") {
        return (*other).ToString() + " + " + (*another).ToString();
    } else if (type == "-") {
        return (*other).ToString() + " - " + (*another).ToString();
    } else if (type == "*") {
        return "(" + (*other).ToString() + ") * (" + (*another).ToString() + ")";
    } else if (type == "/") {
        return "(" + (*other).ToString() + ") / (" + (*another).ToString() + ")";
    } else {
        throw std::logic_error("Wrong type of operation");
    }
}

Exponential::Exponential(std::vector<double> _factor) {
    if (!_factor.empty())
        factor = _factor[0];
    else
        factor = 0;
}

double Exponential::Derivative(double num) {
    return (factor * exp(factor * num));
}

double Exponential::operator()(double num) {
    return (exp(factor * num));
}

std::string Exponential::ToString() const {
    if (factor == 0)
        return "1";
    else
        return "exp(" + to_string(factor, precision) + "*x)";
}

std::shared_ptr<TFunc> Exponential::Copy() {
    return std::make_shared<Exponential>(*this);
}

Polynomial::Polynomial() {
    Coefficients[0] = 0;
}

Polynomial::Polynomial(double coefficients) {
    Coefficients[0] = coefficients;
}

Polynomial::Polynomial(std::vector<double> coefficients) {
    for (int idx = 0; idx < coefficients.size(); idx++)
        Coefficients[idx] = coefficients[idx];
}

double Polynomial::Derivative(double num) {
    double pointDerivative = 0;
    for (auto &[power, factor]: Coefficients) {
        if (power > 0)
            pointDerivative += power * factor * pow(num, power - 1);
    }
    return pointDerivative;
}

double Polynomial::operator()(double num) {
    double res = 0;
    for (auto &[power, factor]: Coefficients) {
        if (factor != 0)
            res += factor * pow(num, power);
    }
    return res;
}

std::string Polynomial::ToString() const {
    std::string res = "";
    for (auto &[key, val]: Coefficients) {
        if (val != 0)
            if (key == 0)
                res += to_string(val, precision) + " + ";
            else
                res += to_string(val, precision) + "*x**" + to_string(key, precision) + " + ";
    }
    return res.substr(0, res.length() - 3);
}

std::shared_ptr<TFunc> Polynomial::Copy() {
    return std::make_shared<Polynomial>(*this);
}

Identity::Identity() {
    Coefficients[1] = 0;
}

Identity::Identity(double coefficients) {
    Coefficients[1] = coefficients;
}

Identity::Identity(std::vector<double> coefficients) {
    Coefficients[1] = coefficients[0];
}

Constant::Constant() {
    Coefficients[0] = 0;
}

Constant::Constant(double coefficients) {
    Coefficients[0] = coefficients;
}

Constant::Constant(std::vector<double> coefficients) {
    if (!coefficients.empty())
        Coefficients[0] = coefficients[0];
    else
        Coefficients[0] = 0;
}

Power::Power() {
    Coefficients[0] = 0;
}

Power::Power(double coefficients) {
    Coefficients[coefficients] = 1;
}

Power::Power(std::vector<double> coefficients) {
    for (int idx = 0; idx < coefficients.size(); idx++)
        Coefficients[coefficients[idx]] = 1;
}