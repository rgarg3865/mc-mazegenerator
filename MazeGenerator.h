#ifndef MAZEGENERATOR
#define MAZEGENERATOR

#include </usr/local/include/mcpp/mcpp.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#include "StackMan.h"

using std::cout, std::endl;

class MazeGenerator
{
   public:
    MazeGenerator();
    MazeGenerator(int, int, bool);
    ~MazeGenerator();
    void generateMaze();
    char** getMazeMask();
    mcpp::Coordinate getExit();

   protected:
    enum compass { north, east, south, west, none };
    compass direction = none;
    int xLen, zLen;
    bool testmode;
    int x = 0, z = 0;
    mcpp::Coordinate exit;
    bool* directions;
    char** mazeMask;
    StackMan<Coord2D> stack;
    Coord2D currCoord;

    //    Functions
    void init();
    bool* getAvailableDirections();
    compass randomDirection(const bool*);
    bool breakBlocks(compass);
    bool move(compass);
    void dump(bool);

    template <typename T, typename... Types>
    void debugPrint(T var1, Types... var2) {
        print("-----Begining debug printing-----");
        print(var1, var2...);
        print("-----Finished debug printing-----");
    }
    void print(){}
    template <typename T, typename... Types>
    void print(T var1, Types... var2) {
        cout << var1 << endl;
        print(var2...);
    }

};

#endif  // ASSIGN_MAZE_H
