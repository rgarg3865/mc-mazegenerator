#include "MazeValidation.h"

MazeValidation::MazeValidation(char** mask, int length, int width, Coord exit):
    mask(mask),
    invertedMask(nullptr),
    length(length),
    width(width),
    exit(exit) {
    }

void MazeValidation::makeCorrections(Coord start, char** maze){
    bool isPerfect = true;
    do{
        bool edgeB = false;
        bool edgeT = false;
        bool edgeR = false;
        bool edgeL = false;
        bool madeCorrection = false;

        isPerfect = true;
        floodFill(start, maze);
        // std::cout << "flood filled maze" << std::endl;
        printMaze(maze);
        for(int i = 0; i < length; i++){
            for(int j = 0; j < width; j++){
                floodFill(start, maze);
                if(maze[i][j] == '.' && !madeCorrection){
                    madeCorrection = true;
                    // std::cout << "making correction" << std::endl;
                    isPerfect = false;
                    int iIndex = i;
                    int jIndex = j;
                    bool dirFound = false;
                    while(!dirFound){
                        if(iIndex == 1) {edgeT = true;}
                        if(jIndex == 1) {edgeL = true;}
                        if(iIndex == length-2){edgeB = true;}
                        if(jIndex == width -2){edgeR = true;}
                        int direction = rand() % 4;
                        if(direction == 0 && !edgeT){
                            if(maze[iIndex-1][jIndex] == '.'){
                                --iIndex;
                            }
                            else{
                                maze[iIndex-1][jIndex] = '.';
                                dirFound = true;
                                // std::cout << "North" << std::endl;
                            }
                        }
                        else if(direction == 3 && !edgeL){
                            if(maze[iIndex][jIndex-1] == '.'){
                                --jIndex;
                            }
                            else{
                                maze[iIndex][jIndex-1] = '.';
                                dirFound = true;
                                // std::cout << "West" << std::endl;
                            }
                        }
                        else if(direction == 1 && !edgeR){
                            if(maze[iIndex][jIndex+1] == '.'){
                                ++jIndex;
                            }
                            else{
                                maze[iIndex][jIndex+1] = '.';
                                dirFound = true;
                                // std::cout << "East" << std::endl;
                            }
                        }
                        else if(direction == 2 && !edgeB){
                            if(maze[iIndex+1][jIndex] == '.'){
                                ++iIndex;
                            }
                            else{
                                maze[iIndex+1][jIndex] = '.';
                                dirFound = true;
                                // std::cout << "South" << std::endl;
                            }
                        }
                        edgeB = false;
                        edgeT = false;
                        edgeR = false;
                        edgeL = false;
                    }
                    
                    madeCorrection = true;
                }
            }
        }
        starToDot(maze);
    } while(!isPerfect);
    // std::cout << "perfect" << std::endl;
}

char** MazeValidation::getPerfectMaze(){
    bool perfected = true;
    do {
        Coord start = {0,0};
        makeCorrections(exit, mask);
        invertedMask = invertMask(mask);
        makeCorrections(start, invertedMask);
        mask = invertMask(invertedMask);
        perfected = checkFixes(mask);
    } while (!perfected);

    return mask;
}

bool MazeValidation::checkFixes(char** maze){
    
    bool toReturn = false;
    bool exitFunc = false;
    
    Coord start = {0,0};
    floodFill(exit, maze);
    for(int i=0; i<length; ++i){
        for(int j=0; j<width; ++j){
            if(maze [i][j] == '.'){
                // std::cout << "Isolation found, Restarting fixing...";
                toReturn = false;
                exitFunc = true;
                j = width - 1;
                i = length - 1;
            }
        }
    }
    if (!exitFunc) {
        starToDot(maze);
        invertedMask = invertMask(mask);
        floodFill(start, invertedMask);
        for(int i=0; i<length; ++i){
            for(int j=0; j<width; ++j){
                if(invertedMask[i][j] == '.'){
                    // std::cout << "Loop found, Restarting fixing...";
                    toReturn = false;
                    exitFunc = true;
                    j = width - 1;
                    i = length - 1;
                }
            }
        }
    }
    if (!exitFunc) {
        starToDot(maze);
        maze = invertMask(invertedMask);
        // std::cout << "No errors found" << std::endl;
        toReturn = true;
        exitFunc = true;
    }
    
    starToDot(maze);
    return toReturn;
}    

char** MazeValidation::invertMask(char** toConvert){
    char** invMask = new char*[length];
    for (int i = 0; i < length; i++) {
        invMask[i] = new char[width];
        for (int j = 0; j < width; j++) {
            if(toConvert[i][j] == '.'){
                invMask[i][j] = 'x';
            }
            else if(toConvert[i][j] == 'x'){
                invMask[i][j] = '.';
            }
        }
    }
    return invMask;
}

void MazeValidation::starToDot(char** maze){
    for(int i=0; i < length; i++){
        for(int j=0; j < width; j++){
            if(maze[i][j] == '*'){
                maze[i][j] = '.';
            }
        }
    }
}

void MazeValidation::floodFill(Coord curr, char** maze) {
    
    if (curr.x >= 0 && curr.x < length && curr.z >= 0 && curr.z < width && maze[curr.x][curr.z] == '.'){
        maze[curr.x][curr.z] = '*';

        Coord nextCell;
        nextCell= {curr.x + 1, curr.z};
        floodFill(nextCell, maze);

        nextCell = {curr.x, curr.z + 1};
        floodFill(nextCell, maze);

        nextCell = {curr.x - 1, curr.z};
        floodFill(nextCell, maze);

        nextCell = {curr.x, curr.z - 1};
        floodFill(nextCell, maze);
    }
}

void MazeValidation::printMaze(char** maze){
    for(int i=0; i < length; i++){
        for(int j=0; j < width; j++){
            // std::cout << maze[i][j];
        }
        // std::cout << std::endl;
    }
}