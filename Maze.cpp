#include "Maze.h"

#include <iostream>
#include <memory>
#include <vector>

Maze::~Maze() {}

// Default constructor to initialise every variable
Maze::Maze(std::shared_ptr<mcpp::MinecraftConnection> mc, bool testmode,
           bool fastmode)
    : mc(mc), testmode(testmode), fastmode(fastmode), xLen(0), zLen(0),
      basePoint(0, 0, 0), carpet(0, 0, 0), mazeMask(nullptr),
      terraform(nullptr), nonTerraformed(nullptr), canTerraform(false),
      isMazeGenerated(false), isTerraformedMazeBuilt(false),
      isNonTerraformedMazeBuilt(false), isMazeRead(false), isTerraformedMazeRestored(true), isNonTerraformedMazeRestored(true),
      delay(0),
      hMap(mc->getHeights(mcpp::Coordinate(1, 0, 0), mcpp::Coordinate(0, 0, 0)))

{
  this->delay = ((this->fastmode) ? 0 : 50);
  terraform = std::make_unique<Terraform>(mc, fastmode);
}

void Maze::setBasePoint(mcpp::Coordinate basePoint) {
  this->basePoint = basePoint;
}

// Individually set each variable required for the class
void Maze::setXlen(int xLen) { this->xLen = xLen; }
void Maze::setZlen(int zLen) { this->zLen = zLen; }
void Maze::setCanTerraform(bool canTerraform) {
  this->canTerraform = canTerraform;
}

bool Maze::getCanTerraform() { return this->canTerraform; }

void Maze::setExit(mcpp::Coordinate exit) { this->carpet = exit; }

// Get valid positions (non-solid blocks) to teleport the player to
std::vector<mcpp::Coordinate> Maze::getValidPlayerPositions() {
  std::vector<mcpp::Coordinate> validBlocks;
  for (int z = 1; z < zLen - 1; ++z) {
    for (int x = 1; x < xLen - 1; ++x) {
      if (mazeMask[z][x] == '.') {
        validBlocks.push_back(mcpp::Coordinate(basePoint.x + 1 + x, basePoint.y,
                                               basePoint.z + z));
      }
    }
  }
  return validBlocks;
}

// Move the player to the correct location, considering testmode, making sure
// that the maze is both generated and built, and (for user entered maze), a
// valid block exists.
void Maze::teleportPlayer() {
  std::vector<mcpp::Coordinate> teleportValidBlocks = getValidPlayerPositions();
  if ((isMazeGenerated || isMazeRead) &&
      (isTerraformedMazeBuilt || isNonTerraformedMazeBuilt)) {
    if (!teleportValidBlocks.empty()) {
      if (testmode) {
        mcpp::Coordinate furthestAirBlock =
            teleportValidBlocks.at(teleportValidBlocks.size() - 1);
        int height = mc->getHeight(furthestAirBlock.x, furthestAirBlock.z) + 1;
        mc->setPlayerPosition(
            mcpp::Coordinate(furthestAirBlock.x, height, furthestAirBlock.z));
        std::cout << "\nTeleported the player to the furthest "
                     "coordinate from the basepoint"
                     " in the "
                     "maze!\nLocation: "
                  << furthestAirBlock << std::endl;
      } else {
        int random = rand() % teleportValidBlocks.size();
        mcpp::Coordinate randomAirBlock = teleportValidBlocks.at(random);
        int height = mc->getHeight(randomAirBlock.x, randomAirBlock.z) + 1;
        mc->setPlayerPosition(
            mcpp::Coordinate(randomAirBlock.x, height, randomAirBlock.z));
        std::cout << "\nTeleported the player to a random location in the "
                     "maze!\nLocation: "
                  << randomAirBlock << std::endl;
      }
    } else {
      std::cout << std::endl
                << "IMPOSSIBLE! THE MAZE DOES NOT CONTAIN ANY PATHS!"
                << std::endl;
    }
  } else if ((isMazeGenerated || isMazeRead) &&
             (!isTerraformedMazeBuilt || isNonTerraformedMazeBuilt)) {
    std::cout << "\nTO SOLVE, PLEASE BUILD THE MOST RECENT MAZE USING "
                 "MAIN MENU "
                 "OPTION 2"
              << std::endl;
  } else {
    std::cout << "\nPLEASE GENERATE AND BUILD A MAZE FIRST!" << std::endl;
  }
}

mcpp::Coordinate Maze::getPlayerPosition() { return mc->getPlayerPosition(); }

// Find out if the player is in the maze
bool Maze::playerInMaze() {
  mcpp::Coordinate playerPosition = mc->getPlayerPosition();
  std::vector<mcpp::Coordinate> validPlayerPositions =
      getValidPlayerPositions();
  bool inMaze = false;

  for (unsigned int i = 0; i < validPlayerPositions.size(); ++i) {
    if (playerPosition.x == validPlayerPositions.at(i).x &&
        playerPosition.z == validPlayerPositions.at(i).z) {
      inMaze = true;
    }
  }
  return inMaze;
}

// Call the correct class to generate the maze, passing required varibles
// eg. if terraforming is allowed, testmode
void Maze::generateMaze() {
  hMap = mc->getHeights(basePoint + mcpp::Coordinate(1, 0, 0),
                        basePoint + mcpp::Coordinate(1 + xLen, 0, zLen));

  isMazeGenerated = true;

  if (canTerraform) {
    MazeGenerator terraformed(xLen, zLen, testmode);
    terraformed.generateMaze();
    mazeMask = terraformed.getMazeMask();
    carpet = terraformed.getExit();
    isTerraformedMazeBuilt = false;
  } else {
    nonTerraformed =
        std::make_unique<NoTerraform>(mc, basePoint, xLen, zLen, testmode);
    nonTerraformed->generate();
    mazeMask = nonTerraformed->getMazeMask();
    carpet = nonTerraformed->getExit();
    isNonTerraformedMazeBuilt = false;
  }
  std::cout << "\nMaze generated successfully!" << std::endl;
  printMaze();
}

void Maze::setMazeMask(char **mazeMask) {
  isMazeRead = true;
  this->mazeMask = mazeMask;

  std::cout << "\nMaze read successfully!" << std::endl;
}

void Maze::printMaze() {

  std::cout << "\n**Printing Maze**\n" << std::endl;
  for (int z = 0; z < zLen; z++) {
    for (int x = 0; x < xLen; x++) {
      std::cout << mazeMask[z][x];
    }
    std::cout << std::endl;
  }
  std::cout << "\n**End Printing Maze**\n" << std::endl;
  std::cout << "Base Point: " << basePoint << std::endl;
}

void Maze::buildMaze() {
  mcpp::BlockType block(0);
  if (!isTerraformedMazeRestored || !isNonTerraformedMazeRestored) {
    restore();
  }

  if (isMazeGenerated || isMazeRead) {
    if (canTerraform) {
        isTerraformedMazeRestored = false;
      isTerraformedMazeBuilt = true;
      std::cout << std::endl << "Terraforming the area..." << std::endl;
      terraform->terraformArea(basePoint, xLen, zLen);
      std::cout << std::endl << "Building the Maze..." << std::endl;
      int c = 0;
      for (int z = 0; z < zLen; z++) {
        for (int x = 0; x < xLen; x++, c++) {
          if (mazeMask[z][x] == 'x') {
            block = mcpp::BlockType(5, 4);
          } else {
            block = mcpp::BlockType(0);
          }
          mc->setBlocks(basePoint + mcpp::Coordinate(x + 1, 0, z),
                        basePoint + mcpp::Coordinate(x + 1, 2, z), block);
          if (c == 5000) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            c = 0;
          }

          std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
      }
    } else {
    isNonTerraformedMazeRestored = false;
      isNonTerraformedMazeBuilt = true;
      std::cout << std::endl << "Building the Maze..." << std::endl;
      basePoint.x++;
      mcpp::HeightMap hMap = nonTerraformed->getHeightMap();
      mcpp::Coordinate columnLow(0, 0, 0);
      mcpp::Coordinate columnHigh(0, 0, 0);
      int yBase = hMap.get(0, 0);
      int maxH = nonTerraformed->getMaxWHeight() + 3;
      mcpp::BlockType block(5, 4);

      for (int z = 0; z < zLen; z++) {
        for (int x = 0; x < xLen; x++) {
          yBase = hMap.get(x, z) + 1;
          if (yBase <= maxH) {
            if (mazeMask[z][x] == 'x') {
              block = mcpp::BlockType(5, 4);
            } else {
              block = mcpp::BlockType(0);
            }
            columnLow =
                mcpp::Coordinate(basePoint.x + x, yBase, basePoint.z + z);
            columnHigh =
                mcpp::Coordinate(basePoint.x + x, maxH, basePoint.z + z);

            mc->setBlocks(columnLow, columnHigh, block);
            if (!fastmode) {
              std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            //
          }
        }
      }
      basePoint.x--;
    }
    std::cout << std::endl
              << "Maze built successfully at " << basePoint << "!" << std::endl;
    mcpp::BlockType blueCarpet = mcpp::Blocks::BLUE_CARPET;
    mc->setBlock(mcpp::Coordinate(basePoint.x + carpet.x + 1,
                                  mc->getHeight(basePoint.x + carpet.x + 1,
                                                basePoint.z + carpet.z) +
                                      1,
                                  basePoint.z + carpet.z),
                 blueCarpet);
  } else {
    std::cout << "\nPLEASE GENERATE A MAZE FIRST USING MAIN MENU OPTION 1!"
              << std::endl;
  }
}

// Call the correct class to solve the maze (Right Hand follow / BFS)
void Maze::solveMaze(solveAlgorithm algo) {
  if (isMazeGenerated || isMazeRead) {
    if (isTerraformedMazeBuilt || isNonTerraformedMazeBuilt) {
      if (playerInMaze()) {
        Agent agent(mc, mc->getPlayerPosition(), testmode, fastmode,
                    canTerraform);
        if (algo == RIGHT_HAND_FOLLOW) {
          agent.solveRightHandFollow();
        } else if (algo == BREATH_FIRST_SEARCH) {
          agent.solveBFS();
        }
      } else {
        std::cout << "\nPLAYER NOT FOUND INSIDE THE MAZE. YOU CAN USE "
                     "SOLVE MENU "
                     "OPTION 1 TO TELEPORT THE PLAYER INSIDE THE MAZE!"
                  << std::endl;
      }
    } else {
      std::cout << "\nTO SOLVE, PLEASE BUILD THE MOST RECENT MAZE USING MAIN "
                   "MENU OPTION 2!"
                << std::endl;
    }
  } else {
    std::cout << "\nPLEASE GENERATE AND BUILD A MAZE FIRST!" << std::endl;
  }
}

// Restore the maze from the maze mask from the correct method,
// taking into account if terraforming was done
void Maze::restore() {

  if (!isTerraformedMazeRestored) {
    std::cout << std::endl << "Restoring the world..." << std::endl;
    terraform->terraformedAreaRestore();
    isTerraformedMazeBuilt = false;
  } else if (!isNonTerraformedMazeRestored) {
    std::cout << std::endl << "Restoring the world..." << std::endl;
    basePoint.x++;
    mcpp::Coordinate columnLow(0, 0, 0);
    mcpp::Coordinate columnHigh(0, 0, 0);
    int yBase = 0;
    int maxH = nonTerraformed->getMaxWHeight() + 3;

    for (int z = 0; z < zLen; z++) {
      for (int x = 0; x < xLen; x++) {
        yBase = hMap.get(x, z) + 1;
        if (yBase <= maxH) {
          columnLow = mcpp::Coordinate(basePoint.x + x, yBase, basePoint.z + z);

          columnHigh = mcpp::Coordinate(basePoint.x + x, maxH, basePoint.z + z);

          mc->setBlocks(columnLow, columnHigh, mcpp::Blocks::AIR);
          if (!fastmode) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
          }
        }
      }
    }
    basePoint.x--;
    isNonTerraformedMazeBuilt = false;
  }
    isTerraformedMazeRestored = true;
    isNonTerraformedMazeRestored = true;
}
