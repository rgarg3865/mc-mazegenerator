#include "NoTerraform.h"
// NoTerraform::NoTerraform() : mc(nullptr), xLen(0), zLen(0);

NoTerraform::NoTerraform(std::shared_ptr<mcpp::MinecraftConnection> mc,
                         mcpp::Coordinate basePoint, int xLen, int zLen,
                         bool testmode)
    : mc(mc),
      basePoint(basePoint),
      heightMap(
          mc->getHeights(basePoint + mcpp::Coordinate(1, 0, 0),
                         basePoint + mcpp::Coordinate(1 + xLen, 0, zLen)))
{
    this->xLen = xLen;
    this->zLen = zLen;
    this->testmode = testmode;
    /*this->heightMap = std::make_unique<mcpp::HeightMap>(mc->getHeights(*/
    /*    basePoint + mcpp::Coordinate(1, 0, 0),*/
    /*    basePoint + mcpp::Coordinate(1 + xLen, 0, zLen)));*/
    int seed = 0;

    if (!testmode) {
        seed = time(0);
    }
    //    int seed = 0;
    srand(seed);
}

// Call the MazeGenerator initialisation function
void NoTerraform::init()
{
    MazeGenerator::init();
    //    maxWalkableHeight is set to -120 as it is beneath with void in
    //    minecraft, so it is not possible for the user to build anything there.
    //    Offset is applied then unapplied later;
    maxWalkableHeight = -120 - basePoint.y + 1;
}

// Recursively generate the made
void NoTerraform::generate()
{
    init();
    heightArray = getHeightMap(xLen, zLen);
    while (!stack.isEmpty()) {
        directions = getAvailableDirections(heightArray, x, z);
        // dump(true);
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
    maxWalkableHeight = maxWalkableHeight + basePoint.y - 1;
}

// Get the heightMap of the area for the maze, convert it into a
// heightMap relative to the player's y position
int** NoTerraform::getHeightMap(int xSize, int zSize)
{
    int currheight = 0;

    int** heights = new int*[zSize];
    for (int i = 0; i < zSize; i++) {
        heights[i] = new int[xSize];
    }

    for (int i = 0; i < xSize; i++) {
        for (int j = 0; j < zSize; j++) {
            currheight = heightMap.get(i, j) - basePoint.y + 1;
            heights[j][i] = currheight;
        }
    }

    return heights;
}

// Get which directions are available to carve paths into
// Also track the highest walkable height in the maze for
// building walls later in Maze.cpp
bool* NoTerraform::getAvailableDirections(int** heights, int x, int z)
{
    direction = none;
    bool* dirs = new bool[4];
    for (int i = 0; i < 4; i++) {
        dirs[i] = false;
    }

// For each direction, if two blocks far is a wall, then set that direction as avaiable
// Also consider if the block is reachable by the player.
    if (z - 2 >= 0) {
        if (mazeMask[z - 2][x] == 'x' &&
            abs(heights[z][x] - heights[z - 1][x]) <= 1 &&
            abs(heights[z - 1][x] - heights[z - 2][x]) <= 1) {
            dirs[0] = true;
            direction = north;

            if (std::max(heights[z - 2][x], heights[z - 1][x]) >
                maxWalkableHeight) {
                maxWalkableHeight =
                    std::max(heights[z - 2][x], heights[z - 2][x]);
            }

        }

    }
    if (x + 2 < xLen) {
        if (mazeMask[z][x + 2] == 'x' &&
            abs(heights[z][x + 1] - heights[z][x]) <= 1 &&
            abs(heights[z][x + 2] - heights[z][x + 1]) <= 1) {
            dirs[1] = true;
            direction = east;

            if (std::max(heights[z][x + 2], heights[z][x + 1]) >
                maxWalkableHeight) {
                maxWalkableHeight =
                    std::max(heights[z][x + 2], heights[z][x + 1]);
            }
        }
    }
    if (z + 2 < zLen) {
        if (mazeMask[z + 2][x] == 'x' &&
            abs(heights[z + 1][x] - heights[z][x]) <= 1 &&
            abs(heights[z + 2][x] - heights[z + 1][x]) <= 1) {
            dirs[2] = true;
            direction = south;

            if (std::max(heights[z + 2][x], heights[z + 1][x]) >
                maxWalkableHeight) {
                maxWalkableHeight =
                    std::max(heights[z + 2][x], heights[z + 1][x]);
            }
        }
    }
    if (x - 2 >= 0) {
        if (mazeMask[z][x - 2] == 'x' &&
            abs(heights[z][x - 1] - heights[z][x]) <= 1 &&
            abs(heights[z][x - 2] - heights[z][x - 1]) <= 1) {
            dirs[3] = true;
            direction = west;

            if (std::max(heights[z][x - 2], heights[z][x - 1]) >
                maxWalkableHeight) {
                maxWalkableHeight =
                    std::max(heights[z][x - 2], heights[z][x - 1]);
            }
        }
    }
    return dirs;
}

// Function for debuging, not required for functionality
void NoTerraform::dump()
{
    for (int i = 0; i < zLen; i++) {
        for (int j = 0; j < xLen; j++) {
            cout << mazeMask[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
}

// Function for debuging, not required for functionality
void NoTerraform::dump(bool a)
{
    MazeGenerator::dump(a);
    //    cout << "Height Array" << endl;
    //    for (int i = 0; i < zLen; i++) {
    //        for (int j = 0; j < xLen; j++) {
    //            cout << mazeMask[i][j] << " ";
    //        }
    //        cout << endl;
    //    }
    cout << endl;
    //    cout << endl;
}

int NoTerraform::getMaxWHeight() {
    return maxWalkableHeight; }
mcpp::HeightMap NoTerraform::getHeightMap() {
    return this->heightMap; }
