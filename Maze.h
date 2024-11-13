

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
    const char wallChar = '#';
    const char corridorChar = ' ';
    const int wallNum = -1;
    const int corridorNum = 0;
public:
    vector<vector<Cell>> maze ;


    Maze(const string &filename){
        loadFromFile(filename);
    }

    void loadFromFile(const string &filename);

    Cell& startPoint(int x, int y);

    void startTraffic ();

    void checkTrace(Cell &checkingCell);
};
