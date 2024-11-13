#include "Maze.h"
using namespace std;



void Maze::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Nie udało się otworzyć pliku: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<Cell> row;
        for (char ch : line) {
            int state;
            if (ch == wallChar) {
                state = wallNum;
            } else if (ch == corridorChar) {
                state =corridorNum;
            } else {

            }
            row.emplace_back(state);
        }
        maze.push_back(row);
    }
    file.close();
}


Cell& Maze::startPoint(int x, int y) {
    Cell& startCell = maze[x][y];
    if (startCell.state == corridorNum) {
        return startCell;
    }
    int offset = 1;
    int dx = 0, dy = -1;

    while (true) {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < offset; ++j) {
                x += dx;
                y += dy;
                if (x >= 0 && x < maze.size() && y >= 0 && y < maze[0].size()) {
                    Cell& cell = maze[x][y];
                    if (cell.state == corridorNum) {
                        return cell;
                    }
                }
            }
            std::swap(dx, dy);
            dy = -dy;
        }
        ++offset;
    }
}






void Maze::startTraffic() {
    int startX = maze.size()/2;
    int startY = maze[0].size()/2;
    Cell& startCell = startPoint(startX,startY);






}





