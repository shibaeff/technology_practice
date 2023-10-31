#pragma once
#include <random>
#include <algorithm>


using namespace  std;

template<typename TSchedule>
class Mutation {
public:
    virtual TSchedule mutate(TSchedule schedule) = 0;
};

template<typename TSchedule>
class MyMutation : Mutation<TSchedule> {
public:
    TSchedule mutate(TSchedule schedule) {
        if (schedule.getNumOfCPUs() == 1) {
            return schedule;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> jobsChoiceMaker(1, schedule.getNumOfJobs());
        std::uniform_int_distribution<> cpuChoiceMaker(0, schedule.getNumOfCPUs() - 1);

        for (unsigned long i = 1; 150 * i <= schedule.getNumOfJobs(); i++) {
            unsigned long jobToMove = jobsChoiceMaker(gen);
            removeJob(schedule, jobToMove);
            auto chosenCPU = cpuChoiceMaker(gen);
            if (schedule.cpuJobs[chosenCPU].size() == 0) {
                schedule.cpuJobs[chosenCPU].push_back(jobToMove);
            } else {
                std::uniform_int_distribution<> positionChoiceMaker(0, schedule.cpuJobs[chosenCPU].size());
                unsigned long indexToMove = positionChoiceMaker(gen);
                schedule.cpuJobs[chosenCPU].push_back(jobToMove);
                std::swap(schedule.cpuJobs[chosenCPU][schedule.cpuJobs[chosenCPU].size() - 1],
                          schedule.cpuJobs[chosenCPU][indexToMove]);
            }
        }

        return schedule;
    }

    void removeJob(TSchedule &schedule, unsigned long movingWork) const {
        for (unsigned long j = 0; j < schedule.getNumOfCPUs(); j++) {
            if (std::find(schedule.cpuJobs[j].begin(), schedule.cpuJobs[j].end(), movingWork) != schedule.cpuJobs[j].end()) {
                remove(schedule.cpuJobs[j].begin(), schedule.cpuJobs[j].end(), movingWork);
                schedule.cpuJobs[j].pop_back();
                break;
            }
        }
    }
};