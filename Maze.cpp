#include "Maze.h"


using namespace std;

std::array<RGB, 255> colorPalette;

void initializeColorPalette() {
    std::srand(std::time(nullptr));
    for (int i = 0; i < 255; ++i) {
        colorPalette[i] = { static_cast<short>(std::rand() % 256),
                            static_cast<short>(std::rand() % 256),
                            static_cast<short>(std::rand() % 256) };
    }
}

void Maze::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie udało się otworzyć pliku: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::unique_ptr<Cell>> row;
        for (char ch : line) {
            if (ch == wallChar) {
                row.emplace_back(std::make_unique<Cell>(wallNum));
            } else if (ch == corridorChar) {
                row.emplace_back(std::make_unique<Cell>(corridorNum));
            }
        }
        maze.push_back(std::move(row));
    }
    file.close();
    mazeWidth = maze[0].size();
    mazeHeight = maze.size();
}

void Maze::getStartPoint(unsigned long &y, unsigned long &x) const {
    if (maze[y][x]->state == corridorNum) {
        return;
    }

    int offset = 1;
    int dx = 0, dy = -1;

    while (true) {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < offset; ++j) {
                y += dy;
                x += dx;
                if (y < maze.size() && x < maze[0].size()) {
                    Cell& cell = *maze[y][x];
                    if (cell.state == corridorNum) {
                        return;
                    }
                }
            }
            std::swap(dx, dy);
            dy = -dy;
        }
        ++offset;
    }
}

void Maze::initializePPM(const std::string& ppmFilename, int scale) {
    ppmFile.open(ppmFilename);
    if (!ppmFile.is_open()) {
        std::cerr << "Nie udało się otworzyć pliku PPM: " << ppmFilename << std::endl;
        return;
    }
    ppmFile << "P3\n" << mazeWidth * scale << " " << mazeHeight * scale << "\n255\n";
}

void Maze::drawThreadPath(int y, int x, std::thread::id threadId, int scale) {
    if (!threadColors.contains(threadId)) {
        threadColors[threadId] = colorPalette[threadColors.size() % colorPalette.size()];
    }

    auto color = threadColors[threadId];
    if (ppmFile.is_open()) {
        for (int i = 0; i < scale; ++i) {
            for (int j = 0; j < scale; ++j) {
                unsigned long px = (x * scale + j) + (y * scale + i) * mazeWidth * scale;
                ppmFile.seekp(px * 12);
                ppmFile << color.r << " " << color.g << " " << color.b << " ";
            }
        }
    }
}


void Maze::drawMaze(const Maze& maze) {
    for (int y = 0; y < maze.mazeHeight; ++y) {
        for (int x = 0; x < maze.mazeWidth; ++x) {
            if (maze.maze[y][x]->state == maze.wallNum) {
                std::cout << maze.wallChar;
            } else if (maze.maze[y][x]->state == maze.corridorNum) {
                std::cout << maze.corridorChar;
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}

void Maze::newWorm(int y, int x, std::thread::id parentID, int scale) { {
        Cell& startCell = *maze[y][x];
        std::lock_guard lock(startCell.cell_mutex);
        if (startCell.state == corridorNum) {
            startCell.state = std::hash<std::thread::id>{}(std::this_thread::get_id());
            drawThreadPath(y, x, std::this_thread::get_id(), scale);
            std::cout << "[Start] Wątek: " << std::this_thread::get_id()
                      << " Startowa pozycja: y=" << y << ", x=" << x
                      << ", stan: " << startCell.state << std::endl;
        } else {
            std::cout << "[Info] Wątek: " << std::this_thread::get_id()
                      << " - Pozycja początkowa y=" << y << ", x=" << x
                      << " jest zajęta. Zakończenie wątku." << std::endl;
            return;
        }
    }


    while (true) {
        std::vector<std::pair<int, int>> availableMoves;
        std::cout << "[Debug] Wątek: " << std::this_thread::get_id()
                  << " Sprawdzanie dostępnych kierunków z pozycji y=" << y << ", x=" << x << std::endl;

        {
            std::lock_guard<std::mutex> lock(maze[y][x]->cell_mutex);
            if (y > 0 && maze[y-1][x]->state == corridorNum) {
                availableMoves.emplace_back(y - 1, x);
                std::cout << "[Check] Góra: y-1=" << y-1 << ", x=" << x << " Stan: " << maze[y-1][x]->state << std::endl;
            }
            if (x > 0 && maze[y][x-1]->state == corridorNum) {
                availableMoves.emplace_back(y, x - 1);
                std::cout << "[Check] Lewo: y=" << y << ", x-1=" << x-1 << " Stan: " << maze[y][x-1]->state << std::endl;
            }
            if (y < maze.size() - 1 && maze[y+1][x]->state == corridorNum) {
                availableMoves.emplace_back(y + 1, x);
                std::cout << "[Check] Dół: y+1=" << y+1 << ", x=" << x << " Stan: " << maze[y+1][x]->state << std::endl;
            }
            if (x < maze[0].size() - 1 && maze[y][x+1]->state == corridorNum) {
                availableMoves.emplace_back(y, x + 1);
                std::cout << "[Check] Prawo: y=" << y << ", x+1=" << x+1 << " Stan: " << maze[y][x+1]->state << std::endl;
            }
        }

        if (availableMoves.empty()) {
            std::cout << "[End] Wątek: " << std::this_thread::get_id()
                      << " nie ma dalszych możliwych ruchów. Zakończenie wątku." << std::endl;
            return;
        } {
            auto [nextY, nextX] = availableMoves[0];
            Cell& nextCell = *maze[nextY][nextX];
            std::lock_guard<std::mutex> lock(nextCell.cell_mutex);
            if (nextCell.state == corridorNum) {
                nextCell.state = std::hash<std::thread::id>{}(std::this_thread::get_id());
                drawThreadPath(nextY, nextX, std::this_thread::get_id(), scale);
                std::cout << "[Move] Wątek: " << std::this_thread::get_id()
                          << " Przesunął się na pozycję y=" << nextY << ", x=" << nextX << std::endl;
            } else {
                std::cout << "[Info] Wątek: " << std::this_thread::get_id()
                          << " - Komórka y=" << nextY << ", x=" << nextX << " jest zajęta." << std::endl;
                return;
            }
        }
        for (size_t i = 1; i < availableMoves.size(); ++i) {
            auto [newY, newX] = availableMoves[i]; {
                std::thread newThread(&Maze::newWorm, this, newY, newX, std::this_thread::get_id(), scale);
                std::lock_guard<std::mutex> lock(locker);
                threads.push_back(std::move(newThread));
            }
            std::cout << "[Fork] Wątek: " << std::this_thread::get_id()
                      << " tworzy nowy wątek na pozycji y=" << newY << ", x=" << newX << std::endl;
        }
    }
}


void Maze::checkPosition(int y, int x) const {
    std::cout << "Sprawdzanie współrzędnych (" << y << "," << x << "):" << std::endl;
    std::cout << "Stan w (" << y << "," << x << "): " << maze[y][x]->state << std::endl;

    if (y > 0)
        std::cout << "Góra (" << y-1 << "," << x << "): " << maze[y-1][x]->state << std::endl;
    if (x > 0)
        std::cout << "Lewo (" << y << "," << x-1 << "): " << maze[y][x-1]->state << std::endl;
    if (y < maze.size() - 1)
        std::cout << "Dół (" << y+1 << "," << x << "): " << maze[y+1][x]->state << std::endl;
    if (x < maze[0].size() - 1)
        std::cout << "Prawo (" << y << "," << x+1 << "): " << maze[y][x+1]->state << std::endl;
}

void Maze::startTraffic(int scale) {
    unsigned long startY = maze.size() / 2;
    unsigned long  startX = maze[0].size() / 2;
    getStartPoint(startY, startX);
    initializePPM("maze.ppm", scale);

    std::thread firstThread(&Maze::newWorm, this, startY, startX, std::this_thread::get_id(), scale);
    threads.push_back(move(firstThread));

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    if (ppmFile.is_open()) {
        ppmFile.close();
    }
}





