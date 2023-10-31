#include <chrono>

#include "./code/Generator.hpp"
#include "./code/Schedule.hpp"
#include "./code/Annealing.hpp"

int main(int argc, char *argv[]) {
    bool isSequential1 = false;
    bool isSequential2 = false;
    bool isParallel = false;
    freopen("logs_3","w",stderr);
    for (int i = 1; i < argc; ++i) { // Start from 1 to skip the program name
        std::string arg = argv[i];
        if (arg == "--sequential1") {
            isSequential1 = true;
        } else if (arg == "--sequential2") {
            isSequential2 = true;
        } else if (arg == "--parallel") {
            isParallel = true;
        }
    }
    if (isSequential1) {
        std::cout << FormatString::format("Running sequential version\n");
        auto cpus = {2, 4, 8, 16, 32, 64, 100, 128};
        auto jobs = {100, 200, 400, 800, 1000, 2000, 4000, 8000, 16000, 32000};
        for (int att = 0; att < 5; att++)
        for (auto cpuNumber: cpus) {
            for (auto jobsNumber: jobs) {
                auto name = FormatString::format("sequential_{}_{}.csv", cpuNumber, jobsNumber);
                generateTestUniform("../inputs/" + name, cpuNumber, jobsNumber, 5, 25);
                Schedule iniSchedule("../inputs/" + name, false);

                std::cout << iniSchedule.getCriterion();

                auto starTime = std::chrono::high_resolution_clock::now();

                Temperature initialTemp(Options::WithCauchy, 150);
                Schedule solution = ParallelAlgo<Schedule>(iniSchedule, initialTemp, 1, 100, 1);
                auto finishTime = std::chrono::high_resolution_clock::now();
                auto overallDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(finishTime - starTime).count();
                std::cerr << FormatString::format("cpus:{},jobs:{},threads:{},time:{},start_criteria:{},final_criteria:{}\n",
                                                  cpuNumber, jobsNumber, 1, overallDuration / 1e9, iniSchedule.getCriterion(), solution.getCriterion());
            }
        }
    }
    if (isSequential2) {
        std::cout << FormatString::format("Running sequential version, experiment 2\n");
        auto cpus = {2,32};
        auto jobs = {16000, 32000};
        for (int att = 0; att < 3; att++)
        for (auto cpuNumber: cpus) {
            for (auto jobsNumber: jobs) {
                auto name = FormatString::format("sequential_{}_{}_cauchy.csv", cpuNumber, jobsNumber);
                generateTestUniform("../inputs/" + name, cpuNumber, jobsNumber, 7, 30);
                Schedule iniSchedule("../inputs/" + name, false);

                std::cout << iniSchedule.getCriterion();

                auto starTime = std::chrono::high_resolution_clock::now();

                Temperature initialTemp(Options::WithLog, 150);
                Schedule solution = ParallelAlgo<Schedule>(iniSchedule, initialTemp, 1, 100, 1);
                auto finishTime = std::chrono::high_resolution_clock::now();
                auto overallDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(finishTime - starTime).count();
                std::cerr << FormatString::format("cpus:{},jobs:{},threads:{},time:{},start_criteria:{},final_criteria:{}\n",
                                                  cpuNumber, jobsNumber, 1, overallDuration / 1e9, iniSchedule.getCriterion(), solution.getCriterion());
            }
        }
    }
    if (isParallel) {
        std::cout << FormatString::format("Running parallel version\n");

        unsigned long cpus = 8;
        unsigned long jobs = 16000;

        for (int att = 0; att < 3; att++)
            for (int numOfWorkers = 1; numOfWorkers <= 10; numOfWorkers++) {
                generateTestUniform("../inputs/test1.csv", cpus, jobs, 5, 25);

                Schedule iniSchedule("../inputs/test1.csv", false);

                std::cout << iniSchedule.getCriterion();

                auto t_start = std::chrono::high_resolution_clock::now();

                Temperature StartTemp(Options::WithCauchy, 125);
                Schedule FinalSchedule = ParallelAlgo<Schedule>(iniSchedule, StartTemp, 25, 10, numOfWorkers);
                auto t_end = std::chrono::high_resolution_clock::now();
                auto t_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();

                // std::cout << "Threads:" << numOfWorkers << ",Processors:" << cpus << ",Works:" << jobs << ",Duration:" << t_duration / 1e9 << ",StartCriterion:" << iniSchedule.getCriterion() << ",FinalCriterion:" << FinalSchedule.getCriterion() << "\n";
                std::cout << FormatString::format("cpus:{},jobs:{},threads:{},time:{},start_criteria:{},final_criteria:{}",
                                                  cpus, jobs, numOfWorkers, t_duration / 1e9, iniSchedule.getCriterion(), FinalSchedule.getCriterion());
            }

    }

}