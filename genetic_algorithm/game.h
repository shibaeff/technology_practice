#include <iostream>

typedef int cell;

struct Board {
    cell *Value = nullptr;
    static int defaultSize;
    int Size = defaultSize;

    Board() {
        Value = new cell[Size * Size];
    }

    Board(int size) : Size(size) {
        Value = new cell[Size * Size];
    }


    Board(int size, cell def_val) : Size(size) {
        if (Value == nullptr)
            Value = new cell[Size * Size];
        for (int i = 0; i < Size * Size; i++) {
            Value[i] = def_val;
        }
    }

    void Generate() {
        if (Value == nullptr)
            Value = new cell[Size * Size];
        for (int i = 0; i < Size * Size; i++) {
            Value[i] = rand() % 2;
        }
    }

    void Zero() {
        for (int i = 0; i < Size * Size; i++) {
            Value[i] = 0;
        }
    }

    Board(const Board *board) {
        Size = board->Size;
        if (Value == nullptr)
            Value = new cell[Size * Size];
        for (int i = 0; i < Size * Size; i++) {
            Value[i] = board->Value[i];
        }
    }

    void Copy(const Board *board) {
        Size = board->Size;
        if (Value == nullptr)
            Value = new cell[Size * Size];
        for (int i = 0; i < Size * Size; i++) {
            Value[i] = board->Value[i];
        }
    }

    cell *operator[](const int index) {
        return &Value[index * Size];
    }

    ~Board() {
        if (Value != nullptr)
            delete[] Value;
        Value = nullptr;
    }
};

struct Glider : public Board {
    static const char GLIDER_SIZE = 3;

    Glider(int x);

    ~Glider() {};
};

struct Blinker : public Board {
    static const char BLINKER_HEIGHT = 3;
    static const char BLINKER_WIDTH = 1;

    Blinker(int x);

    ~Blinker() {};
};

class GameOfLife {
public:
    GameOfLife(Board *sh, int num_steps);

    GameOfLife(int num_steps) : maxSteps(num_steps) {

    }

    void print();

    void update();

    char getState(char state, char xCoord, char yCoord, bool toggle);

    void iterate(unsigned int iterations);

    int start(int visual);

    int StartNew(const Board *base_conf, int visualisation) {
        liveSteps = 0;
        success = false;
        isStatic = false;
        world.Copy(base_conf);
        return start(visualisation);
    }

    Board* GetWorld() {
        if (!toggle) {
            return &world;
        } else {
            return &otherWorld;
        }
    }

    Board world;
    bool isStatic = false;
    bool success = false;
    cell liveSteps = 0;
private:

    Board otherWorld;
    bool toggle;
    int maxSteps;
};

GameOfLife::GameOfLife(Board *sh, int num_steps) : world(sh), otherWorld(sh->Size, 0), toggle(true) {
    maxSteps = num_steps;
}

void GameOfLife::print() {
    if (toggle) {
        for (char i = 0; i < world.Size; i++) {
            for (char j = 0; j < world.Size; j++) {
                std::cout << (world[i][j] == 1 ? 'X' : '-');
            }
            std::cout << std::endl;
        }
    } else {
        for (char i = 0; i < world.Size; i++) {
            for (char j = 0; j < world.Size; j++) {
                std::cout << (otherWorld[i][j] == 1 ? 'X' : '-');
            }
            std::cout << std::endl;
        }
    }
    for (char i = 0; i < world.Size; i++) {
        std::cout << '=';
    }
    std::cout << std::endl;
}

void GameOfLife::update() {
    if (toggle) {
        for (char i = 0; i < world.Size; i++) {
            for (char j = 0; j < world.Size; j++) {
                otherWorld[i][j] = GameOfLife::getState(world[i][j], i, j, toggle);
            }
        }
        toggle = !toggle;
    } else {
        for (char i = 0; i < world.Size; i++) {
            for (char j = 0; j < world.Size; j++) {
                world[i][j] =
                        GameOfLife::getState(otherWorld[i][j], i, j, toggle);
            }
        }
        toggle = !toggle;
    }
}

char GameOfLife::getState(char state, char yCoord, char xCoord, bool toggle) {
    char neighbors = 0;
    if (toggle) {
        for (char i = yCoord - 1; i <= yCoord + 1; i++) {
            for (char j = xCoord - 1; j <= xCoord + 1; j++) {
                if (i == yCoord && j == xCoord) {
                    continue;
                }
                if (i > -1 && i < world.Size && j > -1 && j < world.Size) {
                    if (world[i][j] == 1) {
                        neighbors++;
                    }
                }
            }
        }
    } else {
        for (char i = yCoord - 1; i <= yCoord + 1; i++) {
            for (char j = xCoord - 1; j <= xCoord + 1; j++) {
                if (i == yCoord && j == xCoord) {
                    continue;
                }
                if (i > -1 && i < world.Size && j > -1 && j < world.Size) {
                    if (otherWorld[i][j] == 1) {
                        neighbors++;
                    }
                }
            }
        }
    }
    if (state == 1) {
        return (neighbors > 1 && neighbors < 4) ? 1 : 0;
    } else {
        return (neighbors == 3) ? 1 : 0;
    }
}

void GameOfLife::iterate(unsigned int iterations) {
    for (int i = 0; i < iterations; i++) {
        print();
        update();
    }
}

int GameOfLife::start(int visualisation) {
    if (visualisation > -1)
        print();
    while (!isStatic && liveSteps <= maxSteps) {
        liveSteps++;
        isStatic = true;
        if (world[0][0] == -1)
            world[0][0] = 0;
        for (int i = 0; i < world.Size; i++) {
            for (int j = 0; j < world.Size; j++) {
                int neighbors = 0;
                if (i > 0) {
                    neighbors += (j > 0 && world[i - 1][j - 1] != 0 && world[i - 1][j - 1] <= liveSteps) ? 1 : 0;
                    neighbors += (world[i - 1][j] != 0 && world[i - 1][j] <= liveSteps) ? 1 : 0;
                    neighbors += (j + 1 < world.Size && world[i - 1][j + 1] != 0 && world[i - 1][j + 1] <= liveSteps)
                                 ? 1 : 0;
                }
                if (j + 1 < world.Size) {
                    if (world[i][j + 1] == -1)
                        world[i][j + 1] = 0;
                    else
                        neighbors++;
                }

                if (i + 1 < world.Size) {
                    if (j > 0 && world[i + 1][j - 1] != 0) {
                        if (world[i + 1][j - 1] == -1)
                            world[i + 1][j - 1] = 0;
                        else
                            neighbors++;
                    }
                    if (world[i + 1][j] != 0) {
                        if (world[i + 1][j] == -1)
                            world[i + 1][j] = 0;
                        else
                            neighbors++;
                    }
                    if (j + 1 < world.Size && world[i + 1][j + 1] != 0) {
                        if (world[i + 1][j + 1] == -1)
                            world[i + 1][j + 1] = 0;
                        else
                            neighbors++;
                    }
                }

                if (world[i][j] > 0 && (neighbors < 2 || neighbors > 3)) {
                    world[i][j] = -1;
                    isStatic = false;
                } else if (world[i][j] == 0 && neighbors == 3) {
                    isStatic = false;
                }
            }
        }
        if (visualisation > -1)
            print();
    }
    if (!isStatic && liveSteps >= maxSteps)
        success = true;
    return liveSteps;
}

Glider::Glider(int size) : Board(size) {
    (*this)[0][1] = (*this)[1][2] = (*this)[2][0] = (*this)[2][1] = (*this)[2][2] = 1;
}


Blinker::Blinker(int x) : Board(x) {

    for (char i = 2; i < 2 + BLINKER_HEIGHT; i++) {
        for (char j = 2; j < 2 + BLINKER_WIDTH; j++) {
            (*this)[i][j] = 1;
        }
    }
}
