#include "Generate.h"
#include "Maze.h"

int main() {
    const int height = 10;//wielkosc razy 2
    const int width = 10;//wielkosc razy 2
    const char wallChar = '#';
    const char corridorChar = ' ';
    std::string filename= "maze.txt";

    GenMaze genMaze(height, width,wallChar,corridorChar);
    genMaze.generate();
    genMaze.saveToFile(filename);

    Maze maze (filename,wallChar,corridorChar);
    maze.print();

    return 0;
}

