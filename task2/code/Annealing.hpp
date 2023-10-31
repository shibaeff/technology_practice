#ifndef ALGO
#define ALGO

#include <unordered_map>
#include <vector>
#include <thread>
#include <future>
#include <mutex>

#include "Schedule.hpp"
#include "Temperature.hpp"
#include "Format.hpp"
#include "Mutation.hpp"

template<typename GenericSchedule >
class SimAnnealing {
public:
    SimAnnealing(GenericSchedule  initialSchedule, Temperature initialTemp, unsigned long iterations_) :
            startSchedule(initialSchedule), solution(initialSchedule), currentSchedule(initialSchedule),
            temperature(initialTemp), iterations(iterations_) {}

    int runInstance(std::mutex& globalMutex, GenericSchedule & bestSolution, unsigned int intName) {
        instance(intName);
        globalMutex.lock();
        if (bestSolution.getCriterion() > solution.getCriterion()) {
            bestSolution = solution;
        }
        globalMutex.unlock();
        return 0;
    }

    GenericSchedule  instance(unsigned int intName = 0) {
        unsigned long convergenceCounter = 0;
        unsigned long iterCurrent = 0;

        unsigned long long currentCriterion = currentSchedule.getCriterion();
        unsigned long long getCriterion = solution.getCriterion();
        unsigned long long tmpCriterion;

        MyMutation<Schedule> mutation;

        while (convergenceCounter < convergenceConstraint) {
            iterCurrent++;
            for (int i = 0; i < iterations; i++) {
                convergenceCounter++;
                GenericSchedule  tmpSchedule = mutation.mutate(currentSchedule);
                tmpCriterion = tmpSchedule.getCriterion();

                if ((tmpCriterion <= currentCriterion) ||
                    (temperature.isBetterEnough((long long) tmpCriterion - (long long) currentCriterion))) {
                    if (tmpCriterion < getCriterion) {
                        solution = tmpSchedule;
                        getCriterion = solution.getCriterion();
                        convergenceCounter = 0;
                    }
                    currentSchedule = tmpSchedule;
                    currentCriterion = currentSchedule.getCriterion();
                }
            }
            if (intName != 0) {
                std::cout << FormatString::format("{red} THREAD #{}{reset}\n", intName);
            }
            std::cout << FormatString::format("{blue} iter:{}, temp: {}, temp: {}, criteria: {}{reset}\n", iterCurrent, temperature.getCurrentTemp(), iterCurrent, getCriterion);
            temperature.temperatureStep(iterCurrent);
        }

        return solution;
    }
private:
    const unsigned long convergenceConstraint = 100;
    unsigned long iterations;

    GenericSchedule  startSchedule;
    GenericSchedule  currentSchedule;
    GenericSchedule  solution;

    Temperature temperature;

};

template<typename GenericSchedule >
GenericSchedule  ParallelAlgo(GenericSchedule  initialSchedule_, Temperature initialTemp, unsigned long iterations, const unsigned long convergenceThresh, unsigned int procNum) {
    unsigned long currentConvergence = 0;

    GenericSchedule  initialSchedule = initialSchedule_;
    GenericSchedule  protectedSolution = initialSchedule_;
    std::mutex scheduleMutex;

    while (currentConvergence < convergenceThresh) {
        std::vector<SimAnnealing<GenericSchedule >> results;
        std::vector<std::thread> threads;

        for (unsigned int i = 0; i < procNum; i++) {
            results.push_back(SimAnnealing<GenericSchedule >(initialSchedule, initialTemp, iterations));
        }

        for (unsigned int i = 0; i < procNum; i++) {
            threads.push_back(std::thread {
                [&protectedSolution, &results, &scheduleMutex, i] {
                    results[i].runInstance(scheduleMutex, protectedSolution, i + 1);
                }
            });
        }

        for (unsigned int i = 0; i < procNum; i++) {
            threads[i].join();
        }

        if (initialSchedule.getCriterion() == protectedSolution.getCriterion()) {
            currentConvergence++;
        } else {
            currentConvergence = 0;
            initialSchedule = protectedSolution;
        }

        std::cout << FormatString::format("{blue} Current criterion value: {}; convergence: {}\n{reset}",
                                          protectedSolution.getCriterion(), currentConvergence);
    }

    return protectedSolution;
}

#endif