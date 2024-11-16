#include "Generate.h"
#include "Maze.h"

void genNewMaze(int &h,int &w , std::string &f) {
    GenMaze genMaze(h, w);//generowanie labiryntu uzywajac ]algorytmu przeszukiwania w głąb z wykorzystaniem stosu
    genMaze.generate();
    genMaze.saveToFile(f);
    genMaze.saveToPPM("generated_labyrinth.ppm", 10);
}

int takeInput( int& width , int& height , std::string& file , int argc , char* argv[] ) {
    if (argc < 4 || argc > 5) {
        std::cerr << "Użycie: " << argv[0] << " [szerokość] [wysokość] [nazwaPlikuLabiryntu]\n";
        return 1;
    }
    if (argc == 4) {
        height = std::stoi(argv[1]);
        width = std::stoi(argv[2]);
        file = argv[3];
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int height, width;
    std::string filename;

    if (takeInput(width, height, filename, argc, argv) != 0) return -1;

    std::cout << "Generowanie labiryntu o wymiarach: " << width << " x " << height << std::endl;
    genNewMaze(height, width, filename);

    Maze maze(filename);
    std::cout << "Rozpoczęcie ruchu w labiryncie" << std::endl;
    Maze::drawMaze(maze);
    auto start = std::chrono::high_resolution_clock::now();
    maze.startTraffic(10);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Czas trwania ruchu: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;

    return 0;
}




