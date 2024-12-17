//
#include "Agent.h"

#include <memory>

Agent::Agent(std::shared_ptr<mcpp::MinecraftConnection> mc,
             mcpp::Coordinate playerPosition, bool testmode, bool fastmode,
             bool canTerraform)
    : mc(mc),
      currentPosition(playerPosition),
      surroundingArea(getSurroundingArea()),
      orientation(NORTH),
      steps(0),
      testmode(testmode),
      fastmode(fastmode),
      canTerraform(canTerraform)
{
}
// Breadth-First-Search algorithm start
// If the maze is terraformed, the user can see the live traversal of the BFS
// algorithm
void Agent::solveBFS()
{
          std::cout << "\nFinding Shortest Path..." << std::endl;
    Queue<BlockInfo> q;
    std::vector<BlockInfo> visited;
    auto start = std::chrono::high_resolution_clock::now();
    if (fastmode || canTerraform) {
        fastConditions(q, visited);
    } else {
        slowConditions(q, visited);
    }

    currentPosition = visited.at(visited.size() - 1).previousBlock;
    /*std::cout << "End <-- ";*/

    for (int i = visited.size() - 1; i > 0; i--) {
        mc->setBlock(visited.at(i).previousBlock, mcpp::Blocks::AIR);
        if (currentPosition.x == visited.at(i).currentBlock.x &&
            currentPosition.z == visited.at(i).currentBlock.z) {
            /*std::cout << currentPosition << " <--";*/
            placeBlock(mcpp::Blocks::LIME_CARPET);
            currentPosition = visited.at(i).previousBlock;
            steps++;
        }
    }
    mc->setBlock(visited.at(visited.size()-1).currentBlock, mcpp::Blocks::BLUE_CARPET);
    mc->setBlock(visited.at(0).currentBlock, mcpp::Blocks::RED_CARPET);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    if(!testmode){
    std::cout << std::endl;
    std::cout << "Shortest Path found in " << duration.count() << " seconds!\n";
    }
    std::cout << "Steps: " << steps << std::endl;
}

// BFS algorithm implementation with less checks for valid paths.
// SHOULD ONLY BE CALLED IF THE MAZE IS TERRAFORMED
void Agent::fastConditions(Queue<BlockInfo>& q, std::vector<BlockInfo>& visited)
{
    mcpp::Coordinate previousPosition;
    BlockInfo b;
    b.currentBlock = currentPosition;
    b.previousBlock = currentPosition;
    q.push(b);

    while (!isAtExit()) {
        BlockInfo temp = q.pop();
        mc->setBlock(b.previousBlock, mcpp::Blocks::LIGHT_BLUE_CARPET);
        currentPosition = temp.currentBlock;
        surroundingArea = getSurroundingArea();
        if (!search(currentPosition, visited)) {
            // front
            if (surroundingArea.get_worldspace(currentPosition +
                                               mcpp::Coordinate(0, 1, -1)) ==
                mcpp::Blocks::AIR) {
                b.currentBlock = currentPosition + mcpp::Coordinate(0, 0, -1);
                b.previousBlock = currentPosition;
                q.push(b);
            }
            // right
            if (surroundingArea.get_worldspace(currentPosition +
                                               mcpp::Coordinate(1, 1, 0)) ==
                mcpp::Blocks::AIR) {
                b.currentBlock = currentPosition + mcpp::Coordinate(1, 0, 0);
                b.previousBlock = currentPosition;
                q.push(b);
            }
            // back
            if (surroundingArea.get_worldspace(currentPosition +
                                               mcpp::Coordinate(0, 1, 1)) ==
                mcpp::Blocks::AIR) {
                b.currentBlock = currentPosition + mcpp::Coordinate(0, 0, 1);
                b.previousBlock = currentPosition;
                q.push(b);
            }
            // left
            if (surroundingArea.get_worldspace(currentPosition +
                                               mcpp::Coordinate(-1, 1, 0)) ==
                mcpp::Blocks::AIR) {
                b.currentBlock = currentPosition + mcpp::Coordinate(-1, 0, 0);
                b.previousBlock = currentPosition;
                q.push(b);
            }
        }
        visited.push_back(temp);
    }
}

// BFS algorithm implementation with more checks for valid paths.
// It can traverse a non terraformed maze.
// This algorithm doesn't take paths where a real player cannot go through
// without flying  over blocks or jumping over holes. CAN BE CALLED FOR BOTH
// TERRAFORMED AND NON TERRAFORMED MAZE
void Agent::slowConditions(Queue<BlockInfo>& q, std::vector<BlockInfo>& visited)
{
    mcpp::Coordinate previousPosition;
    BlockInfo b;
    BlockInfo temp;
    b.currentBlock = currentPosition;
    b.previousBlock = mcpp::Coordinate(0, 0, 0);
    q.push(b);
    while (!isAtExit()) {
        temp = q.pop();
        currentPosition = temp.currentBlock;
        surroundingArea = getSurroundingArea();
        adjustHeight();
        if (!search(currentPosition, visited)) {
            // front
            if (isPathInFront()) {
                b.currentBlock = currentPosition + mcpp::Coordinate(0, 0, -1);
                b.previousBlock = currentPosition;
                q.push(b);
            }
            // right
            if (isPathOnRight()) {
                b.currentBlock = currentPosition + mcpp::Coordinate(1, 0, 0);
                b.previousBlock = currentPosition;
                q.push(b);
            }
            // back
            if (isPathAtBack()) {
                b.currentBlock = currentPosition + mcpp::Coordinate(0, 0, 1);
                b.previousBlock = currentPosition;
                q.push(b);
            }
            // left
            if (isPathOnLeft()) {
                b.currentBlock = currentPosition + mcpp::Coordinate(-1, 0, 0);
                b.previousBlock = currentPosition;
                q.push(b);
            }
        }
        visited.push_back(temp);
    }
}

// Sets the orientation of the agent at the start of the Right Hand Follow
// Algorithm
void Agent::setOrientation()
{
    if (!isPathOnRight()) {
        orientation = NORTH;
    } else if (!isPathOnLeft()) {
        orientation = SOUTH;
    } else if (!isPathAtBack()) {
        orientation = EAST;
    } else if (!isPathOnLeft()) {
        orientation = WEST;
    } else {
        int random = rand() % 4;
        orientation = static_cast<compass>(random);
    }
}

// Checks if the current position has already been visited
// FOR BREADTH FIRST ALGORITHM
bool Agent::search(mcpp::Coordinate currentPosition,
                   std::vector<BlockInfo> visited)
{
    bool flag = false;

    for (auto i : visited) {
        if (i.currentBlock.x == currentPosition.x &&
            i.currentBlock.z == currentPosition.z) {
            flag = true;
        }
    }
    return flag;
}

// Start of Right Hand Follow algorithm
void Agent::solveRightHandFollow()
{
          std::cout << "\nStarting Wall Follower...\n" << std::endl;
    int delay = 100;
    if (!testmode) {
        setOrientation();
    }
    if (!fastmode) {
        delay = 500;
    }
    while (!isAtExit()) {
        placeBlock(mcpp::Blocks::LIME_CARPET);
        while (!isPathInFront() && !isPathOnRight()) {
            turnAntiClockwise();
        }
        if (isPathOnRight()) {
            turnClockwise();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        placeBlock(mcpp::Blocks::AIR);
        std::cout << "Step[" << steps << "]: " << currentPosition << std::endl;
        move();
        steps++;
    }
          std::cout << "\nPath Shown Succesfully!" << std::endl;
}

// Stores the blocks that are present all around the player
mcpp::Chunk Agent::getSurroundingArea()
{
    mcpp::Coordinate upperCorner = currentPosition + mcpp::Coordinate(1, 3, 1);
    mcpp::Coordinate lowerCorner = currentPosition - mcpp::Coordinate(1, 3, 1);
    return mc->getBlocks(upperCorner, lowerCorner);
}

// FOR RIGHT HAND FOLLOW ALGORITHM
void Agent::turnClockwise()
{
    orientation = static_cast<compass>((orientation + 1) % 4);
}

// FOR RIGHT HAND FOLLOW ALGORITHM
void Agent::turnAntiClockwise()
{
    orientation = static_cast<compass>((orientation + 3) % 4);
}

// Places the given block at the current position of the agent
void Agent::placeBlock(mcpp::BlockType b)
{
    int x = currentPosition.x;
    int y = currentPosition.y;
    int z = currentPosition.z;
    mc->setBlock(mcpp::Coordinate(x, y, z), b);
}

// Checks if the current position of the agent has a BLUE_CARPET
bool Agent::isAtExit()
{
    mcpp::BlockType block = surroundingArea.get_worldspace(currentPosition);
    mcpp::BlockType blockMinusOne = surroundingArea.get_worldspace(
        currentPosition - mcpp::Coordinate(0, 1, 0));
    return block == mcpp::Blocks::BLUE_CARPET ||
           blockMinusOne == mcpp::Blocks::BLUE_CARPET;
}

// Checks if a player can go through a path provided the player cannot jump over
// holes and cannot fly
bool Agent::isTraversable(int x, int z)
{
    std::unique_ptr<mcpp::BlockType[]> ch =
        std::make_unique<mcpp::BlockType[]>(5);
    bool isPath = true;
    for (int i = 0, y = 2; i < 5; i++, y--) {
        ch[i] = surroundingArea.get_worldspace(currentPosition +
                                               mcpp::Coordinate(x, y, z));
    }

    if (ch[1] != mcpp::Blocks::AIR) {
        isPath = false;
    }
    if (ch[0] != mcpp::Blocks::AIR && ch[1] == mcpp::Blocks::AIR &&
        ch[2] != mcpp::Blocks::AIR) {
        isPath = false;
    }
    if (ch[1] == mcpp::Blocks::AIR && ch[2] == mcpp::Blocks::AIR &&
        ch[3] == mcpp::Blocks::AIR && ch[4] == mcpp::Blocks::AIR) {
        isPath = false;
    }
    return isPath;
}

bool Agent::isPathOnRight()
{
    int x = 0, z = 0;
    if (orientation == NORTH) {
        x = 1;
    } else if (orientation == EAST) {
        z = 1;
    } else if (orientation == SOUTH) {
        x = -1;
    } else if (orientation == WEST) {
        z = -1;
    }
    return isTraversable(x, z);
}

bool Agent::isPathOnLeft()
{
    int x = 0, z = 0;
    if (orientation == NORTH) {
        x = -1;
    } else if (orientation == EAST) {
        z = -1;
    } else if (orientation == SOUTH) {
        x = 1;
    } else if (orientation == WEST) {
        z = 1;
    }
    return isTraversable(x, z);
}

bool Agent::isPathAtBack()
{
    int x = 0, z = 0;
    if (orientation == NORTH) {
        x = 0;
        z = 1;
    } else if (orientation == EAST) {
        x = -1;
        z = 0;
    } else if (orientation == SOUTH) {
        x = 0;
        z = -1;
    } else if (orientation == WEST) {
        x = 1;
        z = 0;
    }
    return isTraversable(x, z);
}

bool Agent::isPathInFront()
{
    int x = 0, z = 0;
    if (orientation == NORTH) {
        x = 0;
        z = -1;
    } else if (orientation == EAST) {
        x = 1;
        z = 0;
    } else if (orientation == SOUTH) {
        x = 0;
        z = 1;
    } else if (orientation == WEST) {
        x = -1;
        z = 0;
    }
    return isTraversable(x, z);
}

// Adjusts the height of the agent +-1
void Agent::adjustHeight()
{
    mcpp::BlockType blockZero;
    mcpp::BlockType blockMinusOne;

    blockZero = surroundingArea.get_worldspace(currentPosition);
    blockMinusOne = surroundingArea.get_worldspace(currentPosition +
                                                   mcpp::Coordinate(0, -1, 0));

    if (blockZero != mcpp::Blocks::AIR &&
        blockZero != mcpp::Blocks::BLUE_CARPET) {
        currentPosition.y += 1;
    } else if (blockMinusOne == mcpp::Blocks::AIR) {
        currentPosition.y -= 1;
    }
}

// Moves the agent towards the current orientation
void Agent::move()
{
    if (orientation == NORTH) {
        currentPosition.z -= 1;
    } else if (orientation == EAST) {
        currentPosition.x += 1;
    } else if (orientation == SOUTH) {
        currentPosition.z += 1;
    } else if (orientation == WEST) {
        currentPosition.x -= 1;
    }

    adjustHeight();
    surroundingArea = getSurroundingArea();
}
