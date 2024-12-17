//
#include </usr/local/include/mcpp/mcpp.h>

#include <chrono>
#include <thread>

#include "Queue.h"
#ifndef AGENT
#define AGENT

struct BlockInfo {
    mcpp::Coordinate currentBlock;
    mcpp::Coordinate previousBlock;
};
enum compass { NORTH, EAST, SOUTH, WEST };
enum solveAlgorithm {
    RIGHT_HAND_FOLLOW,
    BREATH_FIRST_SEARCH,
};
class Agent
{
   public:
    Agent(std::shared_ptr<mcpp::MinecraftConnection> mc,
          mcpp::Coordinate playerPosition, bool testmode, bool fastmode,
          bool canTerraform);
    void solveRightHandFollow();
    void solveBFS();

   private:
    std::shared_ptr<mcpp::MinecraftConnection> mc;
    mcpp::Coordinate currentPosition;
    mcpp::Chunk surroundingArea;
    compass orientation;
    int steps;
    bool testmode;
    bool fastmode;
    bool canTerraform;

    // Common Functions for BFS and Right Hand Follow
    void adjustHeight();
    void placeBlock(mcpp::BlockType b);
    mcpp::Chunk getSurroundingArea();
    bool isAtExit();
    bool isTraversable(int x, int z);
    mcpp::BlockType* getFiveBlockChunk(int x, int z);
    bool isPathOnRight();
    bool isPathOnLeft();
    bool isPathInFront();
    bool isPathAtBack();
    bool isAir(mcpp::BlockType block);

    // Functions for BFS
    void slowConditions(Queue<BlockInfo>& q, std::vector<BlockInfo>& visited);
    void fastConditions(Queue<BlockInfo>& q, std::vector<BlockInfo>& visited);
    void setOrientation();
    bool search(mcpp::Coordinate cP, std::vector<BlockInfo> visited);

    // Functions for Right Hand Follow
    void turnClockwise();
    void turnAntiClockwise();
    void move();
};

#endif
