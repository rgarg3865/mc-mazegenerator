#ifndef MENU
#define MENU

#include </usr/local/include/mcpp/mcpp.h>

#include <iostream>
#include <memory>
#include <regex>
#include <vector>

#include "Maze.h"
#include "MazeValidation.h"

class Menu
{
   public:
    Menu(std::shared_ptr<mcpp::MinecraftConnection> mc, bool testmode,
         bool fastmode);
    void start();

   private:
    Maze *maze;
    std::shared_ptr<mcpp::MinecraftConnection> mc;
    bool testmode;
    bool fastmode;
    Terraform terraform;
    std::string mode = "";

    void printStartText(void);
    void printMainMenu(void);
    void printGenerateMazeMenu(void);
    void printSolveMazeMenu(void);
    void printTeamInfo(void);
    void printExitMessage(void);

    void ifEndOfFile(void);

    void clean(std::string &toClean);
    void setValidLengthWidthBP(int &len, int &wid);
    bool isPositionConfirmed(mcpp::Coordinate &basePoint, int &len, int &wid);
    bool checkDimensions(std::string &value);
    char **readMazeFromTerminal(int &len, int &wid, mcpp::Coordinate &carpet);
    void redoMazeInput(std::string &maze, int len);
    bool isPassageTooWide(int &len, int &wid, char **maze);
    void validateUserMaze(char** &maze, int &len, int &wid, mcpp::Coordinate &carpet);
};
#endif
