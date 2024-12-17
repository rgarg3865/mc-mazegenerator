#ifndef MAZE
#define MAZE


#include "Agent.h"
#include "MazeGenerator.h"
#include "NoTerraform.h"
#include "Terraform.h"

class Maze
{
   public:
    ~Maze();
    Maze(std::shared_ptr<mcpp::MinecraftConnection> mc, bool testmode, bool fastmode);

    void setBasePoint(mcpp::Coordinate basePoint);
    void setXlen(int xLen);
    void setZlen(int zLen);
    void setCanTerraform(bool canTerraform);
    bool getCanTerraform();
    void setExit(mcpp::Coordinate exit);
    void teleportPlayer();

    mcpp::Coordinate getPlayerPosition();
    bool playerInMaze();
    void generateMaze();
    void printMaze();
    void setMazeMask(char **mazeMask);
    void buildMaze();
    void solveMaze(solveAlgorithm algo);
    void restore();

   private:
    std::shared_ptr<mcpp::MinecraftConnection> mc;
    bool testmode;
    bool fastmode;
    int xLen, zLen;
    mcpp::Coordinate basePoint;
    mcpp::Coordinate carpet;
    char **mazeMask;
    std::unique_ptr<Terraform> terraform;
    std::unique_ptr<NoTerraform> nonTerraformed;
    bool canTerraform;
    bool isMazeGenerated;
    bool isTerraformedMazeBuilt;
    bool isNonTerraformedMazeBuilt;
    bool isMazeRead;
    bool isTerraformedMazeRestored;
    bool isNonTerraformedMazeRestored;
    int delay;
    mcpp::HeightMap hMap;
    // int maxHeight;
    std::vector<mcpp::Coordinate> getValidPlayerPositions();

    template <typename T, typename... Types>
    void debugPrint(T var1, Types... var2) {
        print("-----Begining debug printing-----");
        print(var1, var2...);
        print("-----Finished debug printing-----");
    }
    template <typename T, typename... Types>
    void print(T var1, Types... var2) {
        std::cout << var1 << std::endl;
        print(var2...);
    }
    void print(){}
};
#endif
