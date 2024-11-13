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
                state = -1; // Ściana
            } else if (ch == corridorChar) {
                state = 0;  // Korytarz
            } else {
                state = ch - '0'; // Wartość numeryczna dla identyfikatora wątku
            }
            row.emplace_back(state);
        }
        maze.push_back(row);
    }
    file.close();
}



