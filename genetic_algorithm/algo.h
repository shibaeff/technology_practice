#include "game.h"

#ifdef OMP
#include <omp.h>
#endif

int sizeOfPopulation;

class Mutation {
public:
    virtual void mutate(Board *pop) = 0;
};

class Crossbreeding {
public:
    virtual Board *crossing(const Board *pop) = 0;
};

class Selection {
public:
    virtual Board *select(const Board *pop, const int *func) = 0;
};


class SurvivalFunc {
public:
    virtual int Survival(Board *board) = 0;
};


class FuncCellAutomat : public SurvivalFunc {
    int max_steps = 100;
public:
    FuncCellAutomat() {}

    int Survival(Board *board) {
        GameOfLife game(board, max_steps);
        game.start(-1);
        int alive = 0;
        for (int i = 0; i < board->Size * board->Size; i++) {
            if (game.world.Value[i] > 0)
                alive++;
        }
        alive = (alive * game.liveSteps) / 100;
        return alive;
    }
};

class RouletteSelection : public Selection {
public:

    Board *select(const Board *pop, const int *func) {
        // roulette selection
        Board *newPopulation = new Board[sizeOfPopulation];
        int f_sum = 0;
        for (int i = 0; i < sizeOfPopulation; i++)
            f_sum += func[i];

        for (int i = 0; i < sizeOfPopulation; i++) {
            int k = rand() % f_sum;
            int old_index = 0;
            int t = 0;
            while (t <= k) {
                t += func[old_index++];
            }
            if (old_index > 0)
                old_index--;
            newPopulation[i].Copy(&pop[old_index]);
        }
        return newPopulation;
    }
};

class PointBreeding : public Crossbreeding {
    double p_crossing = 0.8;

public:
    Board *crossing(const Board *pop) {
        Board *newPopulation = new Board[sizeOfPopulation];
        srand(time(0));
        auto length = pop[0].Size * pop[0].Size;
        for (int i = 0; i < sizeOfPopulation; i += 2) {
            int par1_index = rand() % sizeOfPopulation;
            int par2_index = rand() % sizeOfPopulation;
            double p = static_cast<double>(rand()) / RAND_MAX;

            if (p < p_crossing && i + 1 < sizeOfPopulation) {
                // Perform crossover
                int crossoverPoint = rand() % length;
                for (int j = 0; j < crossoverPoint; j++) {
                    newPopulation[i].Value[j] = pop[par1_index].Value[j];
                    newPopulation[i + 1].Value[j] = pop[par2_index].Value[j];
                }
                for (int j = crossoverPoint; j < length; j++) {
                    newPopulation[i].Value[j] = pop[par2_index].Value[j];
                    newPopulation[i + 1].Value[j] = pop[par1_index].Value[j];
                }
            } else {
                // Copy genes from parents directly if no crossover
                for (int j = 0; j < length; j++) {
                    newPopulation[i].Value[j] = pop[par1_index].Value[j];
                    if (i + 1 < sizeOfPopulation)
                        newPopulation[i + 1].Value[j] = pop[par2_index].Value[j];
                }
            }
        }

        return newPopulation;
    }
};

class BitMutation : public Mutation {
    double Pmut;
public:
    BitMutation(double p) {
        Pmut = p;
    }

    void mutate(Board *pop) {
        for (int i = 0; i < sizeOfPopulation; i++) {
            for (int j = 0; j < pop->Size * pop->Size; j++) {
                double p = float(rand()) / RAND_MAX;
                if (p < Pmut)
                    pop[i].Value[j] = (pop[i].Value[j] == 0) ? 1 : 0;
            }
        }
    }
};

class GeneticAlgo {
    Mutation *mutator;
    Selection *selector;
    Crossbreeding *crosser;
    SurvivalFunc *survivalFunc;

    Board *population;
    int *funcValue;

    Board best;
    int best_value = 0;

    int msi;
    int steps_count;

public:

    GeneticAlgo(Mutation *m, Selection *s, Crossbreeding *c, SurvivalFunc *f, int max_stable_iterations) {
        mutator = m;
        selector = s;
        crosser = c;
        survivalFunc = f;
        msi = max_stable_iterations;
        population = new Board[sizeOfPopulation];
        funcValue = new int[sizeOfPopulation];
        int av = 0;
#ifdef OMP
#pragma omp parallel
#endif
        {
#ifdef OMP
#pragma omp for
#endif
            for (int i = 0; i < sizeOfPopulation; i++) {
                population[i].Generate();
                funcValue[i] = survivalFunc->Survival(&population[i]);
            }
        }
        for (int i = 0; i < sizeOfPopulation; i++) {
            av += funcValue[i];
            if (funcValue[i] > best_value) {
                best.Copy(&population[i]);
                best_value = funcValue[i];
            }
        }
        std::cout << "Generation 0. Average f(i) = " << (1.0 * av) / sizeOfPopulation << ", best result: " << best_value
                  << std::endl;
    }

    void Start() {
        int stable_count = 0;
        steps_count = 0;
        Board *newPopulation;
        while (stable_count < msi) {
            steps_count++;
            std::cout << "Generation " << steps_count << " started.";
            newPopulation = selector->select(population, funcValue);
            delete[] population;
            population = newPopulation;
            newPopulation = crosser->crossing(population);
            delete[] population;
            population = newPopulation;
            mutator->mutate(population);
            int av = 0;
#ifdef OMP
#pragma omp parallel
#endif
            {
#ifdef OMP
#pragma omp for
#endif
                for (int i = 0; i < sizeOfPopulation; i++)
                    funcValue[i] = survivalFunc->Survival(&(population[i]));
            }
            for (int i = 0; i < sizeOfPopulation; i++) {
                if (funcValue[i] > best_value) {
                    best.Copy(&population[i]);
                    best_value = funcValue[i];
                    stable_count = -1;
                }
                av += funcValue[i];
            }
            std::cout << "\tAverage f(i) = " << (1.0 * av) / sizeOfPopulation << ", best result: " << best_value
                      << std::endl;
            stable_count++;
        }
    }

    Board *GetBestboard() {
        return &best;
    }

    int GetBestValue() {
        return best_value;
    }

    int GetStepsCount() {
        return steps_count;
    }

    ~GeneticAlgo() {
        delete[] population;
        delete[] funcValue;
        delete mutator;
        delete selector;
        delete crosser;
        delete survivalFunc;
    }

};
