#ifndef NOTERRAFORM_H
#define NOTERRAFORM_H

#include </usr/local/include/mcpp/mcpp.h>
#include <stdarg.h>
#include <stdio.h>

#include <algorithm>
#include <cmath>

#include "MazeGenerator.h"

using std::abs;
using std::cout, std::endl;
class NoTerraform : public MazeGenerator
{
   public:
    //    NoTerraform();
    NoTerraform(std::shared_ptr<mcpp::MinecraftConnection> mc, mcpp::Coordinate,
                int, int, bool);
    void generate();
    int getMaxHeight();
    mcpp::HeightMap getHeightMap();
    int getMaxWHeight();

   private:
    //    int y=0;
    //
    std::shared_ptr<mcpp::MinecraftConnection> mc;
    mcpp::Coordinate basePoint;
    int** heightArray;
    /*mcpp::HeightMap heightMap;*/
    mcpp::HeightMap heightMap;
    int maxWalkableHeight;

    //    functions
    void init();
    int** getHeightMap(int, int);
    bool* getAvailableDirections(int**, int, int);
    void dump();
    void dump(bool);

    template <typename T, typename... Types>
    void debugPrint(T var1, Types... var2)
    {
        print("-----Begining debug printing-----");
        print(var1, var2...);
        print("-----Finished debug printing-----");
    }
    void print() {}
    template <typename T, typename... Types>
    void print(T var1, Types... var2)
    {
        cout << var1 << endl;
        print(var2...);
    }
};

#endif
