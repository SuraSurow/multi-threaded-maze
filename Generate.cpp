#include "Generate.h"

void GenMaze::initializeMaze() {
    maze.resize(2 * height + 1);
    for (auto& row : maze) {
        row.resize(2 * width + 1);
        for (auto& cell : row) {
            cell = std::make_unique<GenCell>();
        }
    }
}

void GenMaze::removeWalls(int startY, int startX) const {
    std::stack<std::pair<int, int>> stack;
    stack.emplace(startY, startX);
    maze[startY][startX]->visited = true;

    std::random_device rd;
    std::mt19937 g(rd());

    while (!stack.empty()) {
        int y = stack.top().first;
        int x = stack.top().second;
        stack.pop();

        std::vector directions = { Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT };
        std::shuffle(directions.begin(), directions.end(), g);

        for (const Direction dir : directions) {
            int ny = y, nx = x;

            switch (dir) {
                case Direction::UP: ny = y - 2; break;
                case Direction::DOWN: ny = y + 2; break;
                case Direction::LEFT: nx = x - 2; break;
                case Direction::RIGHT: nx = x + 2; break;
            }

            if (ny >= 1 && ny < 2 * height && nx >= 1 && nx < 2 * width && !maze[ny][nx]->visited) {
                setWallProperties(y, x, ny, nx, dir);
                maze[ny][nx]->visited = true;
                stack.emplace(ny, nx);
            }
        }
    }
}

void GenMaze::setWallProperties(int y, int x, int ny, int nx, Direction dir) const {
    switch (dir) {
        case Direction::UP:
            maze[y - 1][x]->visited = true;
            maze[y][x]->top_wall = false;
            maze[y - 1][x]->bottom_wall = false;
            break;
        case Direction::DOWN:
            maze[y + 1][x]->visited = true;
            maze[y][x]->bottom_wall = false;
            maze[y + 1][x]->top_wall = false;
            break;
        case Direction::LEFT:
            maze[y][x - 1]->visited = true;
            maze[y][x]->left_wall = false;
            maze[y][x - 1]->right_wall = false;
            break;
        case Direction::RIGHT:
            maze[y][x + 1]->visited = true;
            maze[y][x]->right_wall = false;
            maze[y][x + 1]->left_wall = false;
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
}

void GenMaze::saveRowToFile(std::ofstream& file, int row) const {
    for (int j = 0; j < 2 * width + 1; j++) {
        file << (maze[row][j]->visited ? corridorChar : wallChar);
    }
    file << "\n";
}

void GenMaze::saveToPPM(const std::string& filename, int scale) const {
    std::ofstream ppmFile(filename);
    if (!ppmFile.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return;
    }

    int ppmWidth = (2 * width + 1) * scale;
    int ppmHeight = (2 * height + 1) * scale;
    ppmFile << "P3\n" << ppmWidth << " " << ppmHeight << "\n255\n";

    for (int row = 0; row < 2 * height + 1; ++row) {
        for (int scaleY = 0; scaleY < scale; ++scaleY) {
            for (int col = 0; col < 2 * width + 1; ++col) {
                rgb color = maze[row][col]->visited ? rgb{255, 255, 255} : rgb{0, 0, 0};
                for (int scaleX = 0; scaleX < scale; ++scaleX) {
                    ppmFile << color.r << " " << color.g << " " << color.b << " ";
                }
            }
            ppmFile << "\n";
        }
    }

    ppmFile.close();
}
