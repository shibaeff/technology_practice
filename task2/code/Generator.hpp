#ifndef GENERATOR
#define GENERATOR

#include <random>
#include <iostream>
#include <fstream>

void generateTestUniform(std::string where, unsigned long numofProc, unsigned long long numOfWorks,
                         unsigned long long left, unsigned long long right) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(left, right);
    std::ofstream of;
    of.open(where);
    if (of.is_open()) {
        of << numofProc << "\n";
        of << numOfWorks << "\n";
        for (int i = 1; i <= numOfWorks; i++) {
            of << i << "," << distrib(gen) << "\n";
        }
    }
    of.close();
}

#endif