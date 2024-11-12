#include "Generate.h"
int main() {
    int height = 10; // Wysokość labiryntu
    int width = 10;  // Szerokość labiryntu

    GenMaze maze(height, width);
    maze.generate();
    maze.saveToFile("maze.txt");

    return 0;
}

