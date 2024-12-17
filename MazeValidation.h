#ifndef MAZEVALIDATION
#define MAZEVALIDATION
#include <iostream>
struct Coord{
    int x,z;
};
class MazeValidation{
    public:
        MazeValidation(char** mask, int length, int width,  Coord exit);
        bool checkFixes(char** maze);
        char** getPerfectMaze();
    private:
        char** mask;
        char** invertedMask;
        int length;
        int width;
        Coord exit;
        void starToDot(char** maze);
        void printMaze(char** maze);
        void floodFill(Coord curr, char** maze);
        void makeCorrections(Coord start, char** maze);
        char** invertMask(char** toConvert);
};
#endif