#ifndef SCHEDULE
#define SCHEDULE

#include <vector>
#include <random>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>


class SolutionInterface {
public:
    virtual unsigned long long getCriterion() = 0;
private:

};

class Schedule : SolutionInterface {
public:
    Schedule(std::string path, bool random);
    unsigned long long getCriterion();
    unsigned long long getNumOfCPUs() const;
    unsigned long long getNumOfJobs() const;
    std::vector<std::vector<unsigned long>> cpuJobs;
private:
    unsigned long numOfCPUs;
    unsigned long numOfJobs;
    std::unordered_map<unsigned long, unsigned long> timeDict;

};

Schedule::Schedule(std::string path, bool random = true) {
    std::stringstream ss;
    std::ifstream iff;
    iff.open(path);
    if (not iff.is_open()) {
        return;
    }
    std::string reader;
    getline(iff, reader);
    ss = std::stringstream(reader);
    ss >> numOfCPUs;
    getline(iff, reader);
    ss = std::stringstream(reader);
    ss >> numOfJobs;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uniformIntDistribution(0, numOfCPUs - 1);
    for (unsigned long i = 0; i < numOfCPUs; i++) {
        cpuJobs.emplace_back();
    }
    for (unsigned long i = 1; i <= numOfJobs; i++) {
        getline(iff, reader);
        ss = std::stringstream(reader);
        getline(ss, reader, ',');
        ss >> timeDict[i];
        if (random) {
            cpuJobs[uniformIntDistribution(gen)].push_back(i);
        } else {
            cpuJobs[0].push_back(i);
        }
    }
    iff.close();
}

unsigned long long Schedule::getCriterion() {
    unsigned long long result_time = 0;
    unsigned long long min, max;
    bool defined = false;
    for (auto &proc : cpuJobs) {
        unsigned long long sum = 0;
        for (auto &task : proc) {
            sum += timeDict[task];
        }
        if (not defined) {
            max = sum;
            min = sum;
            defined = true;
            continue;
        }
        if (sum > max) {
            max = sum;
        }
        if (sum < min) {
            min = sum;
        }
    }
    return max - min;
}

unsigned long long Schedule::getNumOfCPUs() const {
    return numOfCPUs;
}

unsigned long long Schedule::getNumOfJobs() const {
    return numOfJobs;
}

#endif