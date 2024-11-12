//
// Created by bolo on 12.11.24.
//

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
    bool visited ;
    bool top_wall;
    bool bottom_wall ;
    bool left_wall ;
    bool right_wall ;

    GenCell() = default;
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
    std::vector<std::vector<std::unique_ptr<GenCell>>> maze;
    void initializeMaze();
    void removeWalls(int , int);
public:
    GenMaze(int h, int w) : height(h), width(w) {
        initializeMaze();
    }
    void generate();
    void saveToFile(const std::string &) const;


};





#endif //GENCELL_H
