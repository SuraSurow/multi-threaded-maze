#include "Generate.h"
#include <stack>

void GenMaze::initializeMaze() {
    maze.resize(2 * height + 1);
    for (auto& row : maze) {
        row.resize(2 * width + 1);
        for (auto& cell : row) {
            cell = std::make_unique<GenCell>();
        }
    }
}

void GenMaze::removeWalls(int startX, int startY) const {
    std::stack<std::pair<int, int>> stack;
    stack.emplace(startX, startY);
    maze[startX][startY]->visited = true;

    std::random_device rd;
    std::mt19937 g(rd());

    while (!stack.empty()) {
        int x = stack.top().first;
        int y = stack.top().second;
        stack.pop();

        std::vector directions = { Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT };
        std::ranges::shuffle(directions, g);

        for (const Direction dir : directions) {
            int nx = x, ny = y;

            switch (dir) {
                case Direction::UP: nx = x - 2; break;
                case Direction::DOWN: nx = x + 2; break;
                case Direction::LEFT: ny = y - 2; break;
                case Direction::RIGHT: ny = y + 2; break;
            }

            if (nx >= 1 && nx < 2 * height && ny >= 1 && ny < 2 * width && !maze[nx][ny]->visited) {
                setWallProperties(x, y, nx, ny, dir);
                maze[nx][ny]->visited = true;
                stack.emplace(nx, ny);
            }
        }
    }
}

void GenMaze::setWallProperties(int x, int y, int nx, int ny, Direction dir) const {
    switch (dir) {
        case Direction::UP:
            maze[x - 1][y]->visited = true;
            maze[x][y]->top_wall = false;
            maze[x - 1][y]->bottom_wall = false;
            break;
        case Direction::DOWN:
            maze[x + 1][y]->visited = true;
            maze[x][y]->bottom_wall = false;
            maze[x + 1][y]->top_wall = false;
            break;
        case Direction::LEFT:
            maze[x][y - 1]->visited = true;
            maze[x][y]->left_wall = false;
            maze[x][y - 1]->right_wall = false;
            break;
        case Direction::RIGHT:
            maze[x][y + 1]->visited = true;
            maze[x][y]->right_wall = false;
            maze[x][y + 1]->left_wall = false;
            break;
    }
}

void GenMaze::generate() const {
    removeWalls(1, 1);
}

void GenMaze::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku!" << std::endl;
        return;
    }

    for (int i = 0; i < 2 * height + 1; i++) {
        saveRowToFile(file, i);
    }

    file.close();
    //std::cout << "Labirynt zapisany do pliku: " << filename << std::endl;
}

void GenMaze::saveRowToFile(std::ofstream& file, int row) const {
    for (int j = 0; j < 2 * width + 1; j++) {
        file << (maze[row][j]->visited ? corridorChar : wallChar);
    }
    file << "\n";
}






