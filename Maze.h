

#include <mutex>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>


using namespace std;

class Cell {
public :
    int state ; //-1 dla ściany , 0 dla korutarza , >1 to pidy threadow
    //std::mutex mutex ;




};

class Maze {
public:
    vector<vector<Cell>> maze ;
    char wallChar;
    char corridorChar;

    Maze(const string &filename,char wallChar , char corridorChar):wallChar(wallChar),corridorChar(corridorChar){
        loadFromFile(filename);
    }

    void loadFromFile(const string &filename);
    void print();
};
