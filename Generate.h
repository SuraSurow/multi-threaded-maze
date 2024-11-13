#ifndef GENCELL_H
#define GENCELL_H

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <random>
#include <algorithm>

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

class GenMaze {
private:
    int height;
    int width;
    char wallChar;
    char corridorChar;
    std::vector<std::vector<std::unique_ptr<GenCell>>> maze;
    enum class Direction { UP, DOWN, LEFT, RIGHT };

    void initializeMaze();
    void removeWalls(int, int) const;
    void setWallProperties(int x, int y, int nx, int ny, Direction dir) const;
    void saveRowToFile(std::ofstream& file, int row) const;

public:


    GenMaze(int h, int w, char wallChar, char corridorChar)
        : height(h), width(w), wallChar(wallChar), corridorChar(corridorChar) {
        initializeMaze();
    }
    void generate() const;
    void saveToFile(const std::string&) const;
};

#endif //GENCELL_H

