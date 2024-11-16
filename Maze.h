#ifndef MAZE_H
#define MAZE_H

#include <array>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <memory>
#include <iostream>
#include <array>
#include <fstream>
#include <random>
#include <ctime>

class Cell {
public:
    unsigned long state;
    std::mutex cell_mutex;

    explicit Cell(int stateVal = 0) : state(stateVal) {}
    Cell(const Cell&) = delete;
    Cell& operator=(const Cell&) = delete;
};

struct RGB {
    short r, g, b;
};

class Maze {
    const char wallChar = '#';
    const unsigned long corridorChar = ' ';
    const int wallNum = -1;
    const unsigned long corridorNum = 0;
    std::ofstream ppmFile;
    unsigned long mazeWidth{}, mazeHeight{};

public:
    std::vector<std::vector<std::unique_ptr<Cell>>> maze;
    std::vector<std::thread> threads;
    std::map<std::thread::id, std::vector<std::thread::id>> threadTree;
    std::map<std::thread::id, RGB> threadColors;
    std::mutex locker;

    explicit Maze(const std::string &filename) {
        std::cout << "Wczytywanie labiryntu z pliku: " << filename << std::endl;
        loadFromFile(filename);
    }
    ~Maze() = default;

    void checkPosition(int y, int x) const;
    static void drawMaze(const Maze& maze);
    void loadFromFile(const std::string &filename);
    void getStartPoint(unsigned long &y, unsigned long &x) const;
    void startTraffic(int);
    void newWorm(int y, int x, std::thread::id parentID, int);
    void initializePPM(const std::string &filename, int);
    void drawThreadPath(int y, int x, std::thread::id threadId, int);
};

extern std::array<RGB, 255> colorPalette;
void initializeColorPalette();

#endif // MAZE_H


