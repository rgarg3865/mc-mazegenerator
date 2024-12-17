#include "MazeGenerator.h"
#include <chrono>
#include <thread>
MazeGenerator::~MazeGenerator() {}
MazeGenerator::MazeGenerator() : xLen(1), zLen(1), testmode(false) {};

MazeGenerator::MazeGenerator(int xLen, int zLen, bool testmode)
    : xLen(xLen), zLen(zLen), testmode(testmode)
{
    int seed = 0;
    if (!testmode) {
        seed = time(0);
    }
    srand(seed);
}

// Initialise the class variables, setting default values
void MazeGenerator::init()
{
    x = 0;
    z = 0;
    direction = none;
    directions = new bool[4];
    for (int i=0; i<4; i++) {directions[i] = false;}
    mazeMask = new char*[zLen];

    //Initialise the maze with 'x'
    for (int i = 0; i < zLen; i++) {
        mazeMask[i] = new char[xLen];
        for (int j = 0; j < xLen; j++) {
            mazeMask[i][j] = 'x';
        }
    }

    // Choose a random edge of the maze,
    //then choose a random valid entry point on the maze.
    int currRandom = rand() % 4;
    if (currRandom == 0) {
        x = rand() % (xLen / 2);
        x = 2 * x + 1;
        z = 1;
        currCoord = {x, z - 1};
        stack.push(currCoord);
        mazeMask[z - 1][x] = '.';
        exit = mcpp::Coordinate(x, 0, z - 1);

    } else if (currRandom == 1) {
        x = xLen - 1 - 1;
        z = rand() % (zLen / 2);
        z = 2 * z + 1;
        currCoord = {x + 1, z};
        stack.push(currCoord);
        mazeMask[z][x + 1] = '.';
        exit = mcpp::Coordinate(x + 1, 0, z);

    } else if (currRandom == 2) {
        x = rand() % (xLen / 2);
        x = 2 * x + 1;
        z = zLen - 1 - 1;
        currCoord = {x, z + 1};
        stack.push(currCoord);
        mazeMask[z + 1][x] = '.';
        exit = mcpp::Coordinate(x, 0, z + 1);

    } else if (currRandom == 3) {
        x = 1;
        z = rand() % (zLen / 2);
        z = 2 * z + 1;
        currCoord = {x - 1, z};
        stack.push(currCoord);
        mazeMask[z][x - 1] = '.';
        exit = mcpp::Coordinate(x - 1, 0, z);
    }

    currCoord = {x, z};
    stack.push(currCoord);
    mazeMask[z][x] = '.';
}

void MazeGenerator::generateMaze()
{
    if (!testmode) {
        init();
// Recursively generate the maze
        while (!stack.isEmpty()) {
            directions = getAvailableDirections();

            if (direction != none) {
                direction = randomDirection(directions);
                breakBlocks(direction);
                move(direction);
                currCoord = {x, z};
                stack.push(currCoord);

            } else {
                currCoord = stack.pop();
                x = currCoord.x;
                z = currCoord.z;
            }
        }

    } else {
// Testing mode, where init() does not run
// Initialise the class variables, setting default values
// Initialise the maze mask with walls
        mazeMask = new char*[zLen];
        for (int i = 0; i < zLen; i++) {
            mazeMask[i] = new char[xLen];
            for (int j = 0; j < xLen; j++) {
                mazeMask[i][j] = 'x';
            }
        }
        direction = none;
        directions = new bool[4];
        for (int i=0; i<4; i++) {directions[i] = false;}

        x = 1;
        z = 1;
        currCoord = {x - 1, z};
        stack.push(currCoord);
        mazeMask[z][x-1] = '.';
        mazeMask[z][x] = '.';
        exit = mcpp::Coordinate(x-1, 0, z);

// Recursively generate the maze
        while (!stack.isEmpty()) {
            directions = getAvailableDirections();
            //dump(true);
            if (direction != none) {
                if (directions[3]) {
                    direction = west;
                } else if (directions[0]) {
                    direction = north;
                } else if (directions[1]) {
                    direction = east;
                } else if (directions[2]) {
                    direction = south;
                }

                breakBlocks(direction);
                move(direction);
                currCoord = {x, z};
                stack.push(currCoord);

            } else {
                currCoord = stack.pop();
                x = currCoord.x;
                z = currCoord.z;
            }
        }
    }
}

// Find out what directions are available, keeping in mind not to go out of range with mazeMask
bool* MazeGenerator::getAvailableDirections()
{
    direction = none;
    bool* dirs = new bool[4];
    for (int i = 0; i < 4; i++) {
        dirs[i] = false;
    }

// For each direction, if two blocks far is a wall, then set that direction as avaiable

    if (z >= 2) {
        if (mazeMask[z-2][x] == 'x') {
            dirs[0] = true;
            direction = north;
        }
    }

    if (x+2 < xLen) {
        if (mazeMask[z][x+2] == 'x') {
            dirs[1] = true;
            direction = east;
        }
    }

    if (z+2 < zLen) {
        if (mazeMask[z+2][x] == 'x') {
            dirs[2] = true;
            direction = south;
        }
    }

    if (x >= 2) {
        if (mazeMask[z][x-2] == 'x') {
            dirs[3] = true;
            direction = west;
        }
    }

    return dirs;
}

// Choose a random direction to carve. If none available, set the direction as none to signal to the generation loop to pop the stack.
MazeGenerator::compass MazeGenerator::randomDirection(const bool* dirs)
{
    compass dir = none;
    int maxIndex = 4;
    int maxValue = 0;
    int currValue = 0;

    for (int i = 0; i < 4; i++) {
        if (dirs[i]) {
            currValue = (rand() % 100);
        } else {
            currValue = 0;
        }

        if (currValue > maxValue) {
            maxValue = currValue;
            maxIndex = i;
        }
    }

    if (maxIndex == 0) {
        dir = north;
    } else if (maxIndex == 1) {
        dir = east;
    } else if (maxIndex == 2) {
        dir = south;
    } else if (maxIndex == 3) {
        dir = west;
    } else {
        dir = none;
    }

    return dir;
}

// Break blocks in the required direction
bool MazeGenerator::breakBlocks(MazeGenerator::compass dir)
{
    bool result = false;

    if (dir == north && mazeMask[z-2][x] == 'x') {
        mazeMask[z-1][x] = '.';
        mazeMask[z-2][x] = '.';
        result = true;

    } else if (dir == east && mazeMask[z][x+2] == 'x') {
        mazeMask[z][x+1] = '.';
        mazeMask[z][x+2] = '.';
        result = true;

    } else if (dir == south && mazeMask[z+2][x] == 'x') {
        mazeMask[z+1][x] = '.';
        mazeMask[z+2][x] = '.';
        result = true;

    } else if (dir == west && mazeMask[z][x-2] == 'x') {
        mazeMask[z][x-1] = '.';
        mazeMask[z][x-2] = '.';
        result = true;
    }
    return result;
}

// Move the current location in the required direction
bool MazeGenerator::move(MazeGenerator::compass dir)
{
    bool result = false;
    if (dir == north && z >= 2) {
        if (mazeMask[z-1][x] == '.' && mazeMask[z-2][x] == '.') {
            z = z - 2;
            result = true;
        }

    } else if (dir == east && x+2 < xLen) {
        if (mazeMask[z][x+2] == '.' && mazeMask[z][x+2] == '.') {
            x = x + 2;
            result = true;
        }

    } else if (dir == south && z+2 < zLen) {
        if (mazeMask[z+1][x] == '.' && mazeMask[z+2][x] == '.') {
            z = z + 2;
            result = true;
        }

    } else if (dir == west && x >= 2) {
        if (mazeMask[z][x-1] == '.' && mazeMask[z][x-2]) {
            x = x - 2;
            result = true;
        }
    }
    return result;
}

// Return the maze mask
char** MazeGenerator::getMazeMask() { return mazeMask; }
// Return the exit point to set the carpet
mcpp::Coordinate MazeGenerator::getExit() { return exit; }

// Function for debuging, not required for functionality
void MazeGenerator::dump(bool showPointer)
{
    cout << "direction: " << direction << endl;
    cout << "xLen,zLen: " << xLen << "," << zLen << endl;
    cout << "x,z: " << x << "," << z << endl;
    cout << "directions: ";

    for (int i = 0; i < 4; i++) {
        cout << directions[i] << ",";
    }
    cout << endl;

    char temp = mazeMask[z][x];
    mazeMask[z][x] = '*';
    cout << "mazeMask: " << endl;

    for (int i = 0; i < zLen; i++) {
        for (int j = 0; j < xLen; j++) {
            if (showPointer) {
                if (mazeMask[i][j] == '.') {
                    cout << ' ';
                } else if (mazeMask[i][j] == 'x') {
                    cout << 'X';
                } else {
                    cout << mazeMask[i][j];
                }
            } else {
                cout << mazeMask[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
    mazeMask[z][x] = temp;
}

