#include "Generate.h"

void GenCell::display()const {
        std::cout << (visited ? " " : "#");
}



void GenMaze::initializeMaze() {
        maze.resize(2 * height + 1);
        for (auto& row : maze) {
            row.resize(2 * width + 1);
            for (auto& cell : row) {
                cell = std::make_unique<GenCell>();
            }
        }
    }

void GenMaze::removeWalls(int x, int y) {
        maze[x][y]->visited = true;

        enum Direction { UP, DOWN, LEFT, RIGHT };
        std::vector<Direction> directions = { UP, DOWN, LEFT, RIGHT };

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(directions.begin(), directions.end(), g);

        for (Direction dir : directions) {
            int nx = x, ny = y;

            switch (dir) {
                case UP: nx = x - 2; break;
                case DOWN: nx = x + 2; break;
                case LEFT: ny = y - 2; break;
                case RIGHT: ny = y + 2; break;
            }

            if (nx >= 1 && nx < 2 * height && ny >= 1 && ny < 2 * width && !maze[nx][ny]->visited) {
                switch (dir) {
                    case UP:
                        maze[x - 1][y]->visited = true;
                        maze[x][y]->top_wall = false;
                        maze[x - 1][y]->bottom_wall = false;
                        break;
                    case DOWN:
                        maze[x + 1][y]->visited = true;
                        maze[x][y]->bottom_wall = false;
                        maze[x + 1][y]->top_wall = false;
                        break;
                    case LEFT:
                        maze[x][y - 1]->visited = true;
                        maze[x][y]->left_wall = false;
                        maze[x][y - 1]->right_wall = false;
                        break;
                    case RIGHT:
                        maze[x][y + 1]->visited = true;
                        maze[x][y]->right_wall = false;
                        maze[x][y + 1]->left_wall = false;
                        break;
                }
                removeWalls(nx, ny);
            }
        }
    }
void GenMaze::generate() {
        removeWalls(1, 1); // Startowanie generowania od lewego górnego rogu
    }
void GenMaze::saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Nie można otworzyć pliku!" << std::endl;
            return;
        }

        for (int i = 0; i < 2 * height + 1; i++) {
            for (int j = 0; j < 2 * width + 1; j++) {
                if (i % 2 == 0 || j % 2 == 0) {
                    file << (maze[i][j]->visited ? " " : "#");
                } else {
                    file << (maze[i][j]->visited ? " " : "#");
                }
            }
            file << "\n";
        }

        file.close();
        std::cout << "Labirynt zapisany do pliku: " << filename << std::endl;
    }




