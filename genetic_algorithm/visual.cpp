#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>

#include "algo.h"
const int kboardSize = 50;
int Board::defaultSize = kboardSize;
const int DELAY = 500000; // Задержка в микросекундах (0.5 секунды)
const int HEIGHT = 100;    // Высота поля
const int WIDTH = 100;     // Ширина поля

// Функция для отображения поля
void displayField(WINDOW* win, const std::vector<std::vector<int>>& field) {
    for (int y = 0; y < field.size(); y++) {
        for (int x = 0; x < field[y].size(); x++) {
            if (field[y][x] == 1) {
                mvwaddch(win, y, x, 'X'); // Отображаем живую клетку
            } else {
                mvwaddch(win, y, x, ' '); // Отображаем мертвую клетку
            }
        }
    }
    wrefresh(win); // Обновляем окно
}

int main(int argc, char **argv) {
    initscr();              // Инициализируем ncurses
    noecho();               // Отключаем эхо ввода
    curs_set(FALSE);        // Скрываем курсор
    WINDOW* win = newwin(HEIGHT, WIDTH, 0, 0); // Создаем новое окно

    std::ifstream file(argv[1]);
    std::vector<std::string> lines;
    std::string line;

    Board board(50);
    if (file.is_open()) {
        int i = 0;
        while (std::getline(file, line)) {
            for (auto & item: line) {
                int value;
                if (item == 'X') {
                    value = 1;
                } else {
                    value = 0;
                }
                board.Value[i++] = value;
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    GameOfLife gol(&board, 100);


    std::vector<std::vector<int>> buffer(50, std::vector<int>(50, 0));
    std::vector<std::vector<int>> displayBuffer(50, std::vector<int>(50, 0));

    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            mvwaddch(win, i, j, '.');
        }
    }
    auto step = 0;
    char buf[100];
    while (true) {
        auto world = gol.GetWorld();
        for (int i = 0; i < world->Size; i++) {
            for (int j = 0; j < world->Size; j++) {
                buffer[i][j] = (*world)[i][j];
            }
        }
        for (int y = 0; y < world->Size; y++) {
            for (int x = 0; x < world->Size; x++) {
                if (buffer[y][x] != displayBuffer[y][x]) {
                    mvwaddch(win, y, x, buffer[y][x] ? 'X' : '.');
                    displayBuffer[y][x] = buffer[y][x];
                }
            }
        }
        wrefresh(win);
        usleep(DELAY);
        gol.update();
        step++;
        sprintf(buf, "%d", step);
        for (int i = 0; i < strlen(buf); i++) {
            mvwaddch(win, 0, 52 + i, buf[i]);
        }

    }
    endwin();
    return 0;
}