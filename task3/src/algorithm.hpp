//
// Created by Pavel Shibaev on 28.11.23.
//

#ifndef FUNCTIONS_ALGORITHM_HPP
#define FUNCTIONS_ALGORITHM_HPP
#include "Types.hpp"
#include <cmath>

template<typename GenericFunction>
double findRootUsingGradientDescent(GenericFunction &f, double initialGuess, double learningRate=0.1, double tolerance=0.0001, unsigned int maxIterations=1000) {
    double x = initialGuess;
    double h = f(x) / f.Derivative(x);
    int iter = 0;
    while (abs(h) >= tolerance and iter < maxIterations)
    {
        double h = f(x) / f.Derivative(x);
        // x(i+1) = x(i) - f(x) / f'(x)
        x = x - h;
        iter++;
    }
    return x;
}


#endif //FUNCTIONS_ALGORITHM_HPP
