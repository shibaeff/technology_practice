#include<iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <cmath>

#include "algo.h"

#define TEST
// #define OMP

const int kboardSize = 50;
int Board::defaultSize = kboardSize;

void mainloop() {
    sizeOfPopulation = 100;
    int stabilityThresh = 20;
    double pmutInitial = 1.0 / (kboardSize * kboardSize);
    GameOfLife result(100);

    std::ofstream logfile;
    logfile.open("./runs/run.log");
    std::ofstream report;
    std::string fname;

    for (int i = 0; i <= 9; i++) {
        double pmut = pmutInitial * pow(1.5, i);
        for (int j = 0; j <= 9; j++) {

            std::cout << "Running series " + std::to_string(i) + ", run " + std::to_string(j) << '\n';
            auto start = std::chrono::system_clock::now();
            GeneticAlgo algo(new BitMutation(pmut), new RouletteSelection(), new PointBreeding(), new FuncCellAutomat(),
                             stabilityThresh);
            algo.Start();
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "\nJob " << i << ":" << j << " complete\n";
            std::cout << "with time" << elapsed.count() << " sec\n";
            logfile << "experiment" << i << " run " << j << ":\n\tpmut = " << pmut << "; duration = "
                    << elapsed.count();
            logfile << "generations" << algo.GetStepsCount() << "; criterionBest = " << algo.GetBestValue() << "\n";
            Board *bestGameBoard = algo.GetBestboard();
            result.StartNew(bestGameBoard, -1);
            report.open("./runs/series_" + std::to_string(i) + "_run_" + std::to_string(j) + "_sol.txt");
            for (int m = 0; m < 50; m++) {
                for (int n = 0; n < 50; n++)
                    report << (((*bestGameBoard)[m][n] > 0) ? "X" : "-");
                report << "\n";
            }
            report.close();
            report.open("./series_" + std::to_string(i) + "_run_" + std::to_string(j) + "_sol_after100.txt");
            for (int m = 0; m < 50; m++) {
                for (int n = 0; n < 50; n++)
                    report << ((result.world[m][n] > 0) ? "X" : "-");
                report << "\n";
            }
            report.close();
        }
        std::cout << "\nSeries " << i << " finished\n";
    }
}


void test() {
    std::cout << "Running glider:\n";
    auto glider = new Glider(10);
    GameOfLife gol(glider, 100);
    gol.iterate(10);

    std::cout << "\n";
    std::cout << "Running blinker:\n";
    auto blinker = new Blinker(10);
    GameOfLife gol2(blinker, 10);
    gol2.iterate(10);
    delete glider;
    delete blinker;
}

void single_run() {
    sizeOfPopulation = 100;
    int stabilityThresh = 20;
    double pmutInitial = 1.0 / (kboardSize * kboardSize);
    GameOfLife result(100);

    std::ofstream logfile;
    logfile.open("./runs/run.log");
    std::ofstream report;
    std::string fname;
    auto pmut = pmutInitial;
    auto start = std::chrono::system_clock::now();
    GeneticAlgo algo(new BitMutation(pmut), new RouletteSelection(), new PointBreeding(), new FuncCellAutomat(),
                     stabilityThresh);
    algo.Start();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nJob " << " complete\n";
    std::cout << "with time" << elapsed.count() << " sec\n";
    logfile << "experiment \n\tpmutF = " << pmut << "; duration = " << elapsed.count();
    logfile << "generations" << algo.GetStepsCount() << "; criterionBest = " << algo.GetBestValue() << "\n";
    Board *bestGameBoard = algo.GetBestboard();
    result.StartNew(bestGameBoard, -1);
    report.open("./experiment");
    for (int m = 0; m < 50; m++) {
        for (int n = 0; n < 50; n++)
            report << (((*bestGameBoard)[m][n] > 0) ? "X" : "-");
        report << "\n";
    }
    report.close();
    report.open("./experiment_after_100");
    for (int m = 0; m < 50; m++) {
        for (int n = 0; n < 50; n++)
            report << ((result.world[m][n] > 0) ? "X" : "-");
        report << "\n";
    }
    report.close();
}

int main() {
    srand(time(0));
#ifdef TEST
    test();
    single_run();
#else
    mainloop();
#endif
    return 0;
}
