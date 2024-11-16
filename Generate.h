#ifndef GENERATE_H
#define GENERATE_H

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <random>
#include <algorithm>
#include <stack>
#include <random>
#include <fstream>

class GenCell {
public:
    bool visited;
    bool top_wall;
    bool bottom_wall;
    bool left_wall;
    bool right_wall;

    GenCell() : visited(false), top_wall(true), bottom_wall(true), left_wall(true), right_wall(true) {}
    GenCell(const GenCell&) = delete;
    GenCell& operator=(const GenCell&) = delete;
    GenCell(GenCell&&) = default;
    GenCell& operator=(GenCell&&) = default;

    void display() const;
};

struct rgb {
    short r, g, b;
};

class GenMaze {
private:
    const char wallChar = '#';
    const char corridorChar = ' ';
    const int wallNum = -1;
    const int corridorNum = 0;
    int height;
    int width;
    std::vector<std::vector<std::unique_ptr<GenCell>>> maze;
    enum class Direction { UP, DOWN, LEFT, RIGHT };

    void initializeMaze();
    void removeWalls(int startY, int startX) const;
    void setWallProperties(int y, int x, int ny, int nx, Direction dir) const;
    void saveRowToFile(std::ofstream& file, int row) const;

public:
    GenMaze(int h, int w): height(h), width(w) {
        initializeMaze();
    }
    void generate() const;
    void saveToFile(const std::string&) const;
    void saveToPPM(const std::string& filename, int scale = 1) const;
};

#endif //GENERATE_H
