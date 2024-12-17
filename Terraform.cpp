#include "Terraform.h"

Terraform::Terraform(std::shared_ptr<mcpp::MinecraftConnection> mc,
                     bool isFastmode)
    : mc(mc), isFastmode(isFastmode), terrainHeights(getHeightMap())
{
    this->length = 0;
    this->width = 0;
    this->air = mcpp::Blocks::AIR;
}

void Terraform::terraformArea(mcpp::Coordinate basePoint, int length, int width)
{
    this->length = length;
    this->width = width;
    this->basePoint = basePoint;

    // Setting the coordinates for the corners of the area in which the maze is
    // to be built
    setCornerCoordinates();

    // Storing the blocks above the basePoint that are to be restored later
    storeBlocksInChunk(corner, oppUpperCorner);
    if (!blocksInUpperChunk.isEmpty()) {
        fillChunk(corner, oppUpperCorner, air);
    }
    terrainHeights = getHeightMap();

    // Fill the lower chunk upto the height of the basePoint
    terraformLowerChunk();
}

void Terraform::terraformedAreaRestore()
{
    mcpp::Coordinate mazeOppUpperCorner = oppLowerCorner;
    mazeOppUpperCorner.y += 2;
    int delay = 0;
    if (!isFastmode) {
        delay = 50;
    }

    // Removing the maze
    fillChunk(corner, mazeOppUpperCorner, air);

    // Removing the platform built in fastmode
    if (isFastmode) {
        fillChunk(mcpp::Coordinate(corner.x, corner.y - 1, corner.z),
                  mcpp::Coordinate(oppLowerCorner.x, oppLowerCorner.y - 1,
                                   oppLowerCorner.z),
                  air);
    }

    // Restoring the blocks removed above the basePoint
    while (!blocksInUpperChunk.isEmpty()) {
        BlockData block = blocksInUpperChunk.pop();
        mc->setBlock(block.coord, block.type);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    // Removing the blocks filled below basePoint
    if (!isFastmode) {
        restoreFromHeight(basePoint.y);
    }
}

// Removes any blocks placed after the heightmap was generated
void Terraform::restoreFromHeight(int h)
{
    setCornerCoordinates();
    int baseLevel = h - 1;
    mcpp::HeightMap::Iterator itr = terrainHeights.begin();
    for (int x = corner.x; x <= oppLowerCorner.x; x++) {
        for (int z = corner.z; z <= oppLowerCorner.z; z++, itr++) {
            int blockLevel = *itr + 1;
            if (blockLevel < baseLevel) {
                std::cout << "filling chunk  " << x << " " << z  << std::endl;
                std::cout << "baselevel = " << baseLevel << " blockLevel= " << blockLevel << std::endl;
                fillChunk(mcpp::Coordinate(x, baseLevel, z),
                          mcpp::Coordinate(x, blockLevel, z), air);
            }
        }
    }
}

// Raises the blocks below the basePoint and builds a platform for thne maze in
// fastmode
void Terraform::terraformLowerChunk()
{
    int baseLevel = basePoint.y - 1;

    if (isFastmode) {
        mcpp::BlockType yellowWool = mcpp::Blocks::YELLOW_WOOL;
        mcpp::Coordinate c1(corner.x, baseLevel, corner.z);
        mcpp::Coordinate c2(oppLowerCorner.x, baseLevel, oppLowerCorner.z);
        storeBlocksInChunk(c1, c2);

        fillChunk(
            mcpp::Coordinate(corner.x, baseLevel, corner.z),
            mcpp::Coordinate(oppLowerCorner.x, baseLevel, oppLowerCorner.z),
            yellowWool);
    } else {
        mcpp::HeightMap::Iterator itr = terrainHeights.begin();
        for (int x = corner.x; x <= oppLowerCorner.x; x++) {
            for (int z = corner.z; z <= oppLowerCorner.z; z++, itr++) {
                int blockLevel = *itr;
                if (blockLevel < baseLevel) {
                    mcpp::BlockType blockBelow =
                        mc->getBlock(mcpp::Coordinate(x, blockLevel - 1, z));
                    mc->setBlocks(mcpp::Coordinate(x, baseLevel, z),
                                  mcpp::Coordinate(x, blockLevel, z),
                                  blockBelow);
                    // terraform fill was skipping blocks for large mazes if the
                    // delay wasn't long enough, 125ms seems stable after all
                    // our testing
                    std::this_thread::sleep_for(std::chrono::milliseconds(125));
                }
            }
        }
    }
}

mcpp::HeightMap Terraform::getHeightMap()
{
    return mc->getHeights(corner, oppLowerCorner);
}

// Fills the chunk between the two locations with the given block with a delay
// if not in isFastmode
void Terraform::fillChunk(mcpp::Coordinate loc1, mcpp::Coordinate loc2,
                          mcpp::BlockType& block)
{
    if (!isFastmode) {
        for (int i = loc1.x; i <= loc2.x; i++) {
            for (int j = loc1.z; j <= loc2.z; j++) {
                mc->setBlocks(mcpp::Coordinate(i, loc1.y, j),
                              mcpp::Coordinate(i, loc2.y, j), block);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
    } else {
        mc->setBlocks(loc1, loc2, block);
    }
}

// Setting the coordinates for the corners of the area in which the maze is
// to be built
void Terraform::setCornerCoordinates()
{
    corner = mcpp::Coordinate(basePoint.x, basePoint.y, basePoint.z - 1);
    oppLowerCorner =
        mcpp::Coordinate(corner.x + length + 1, corner.y, corner.z + width + 1);
    terrainHeights = getHeightMap();

    int maxHeight = *std::max_element(terrainHeights.begin(), terrainHeights.end());
    oppUpperCorner =
        mcpp::Coordinate(oppLowerCorner.x, maxHeight + 1, oppLowerCorner.z);
}

// Gets the chunk between the specified coordinates and stores it in a queue
void Terraform::storeBlocksInChunk(mcpp::Coordinate& corner,
                           mcpp::Coordinate& oppUpperCorner)
{
    mcpp::Chunk blocks = mc->getBlocks(corner, oppUpperCorner);

    mcpp::Chunk::Iterator itr = blocks.begin();

    for (int y = corner.y; y <= oppUpperCorner.y; y++) {
        for (int x = corner.x; x <= oppUpperCorner.x; x++) {
            for (int z = corner.z; z <= oppUpperCorner.z; z++, itr++) {
                if (*itr != mcpp::Blocks::AIR) {
                    BlockData block = {mcpp::Coordinate(x, y, z), *itr};
                    blocksInUpperChunk.push(block);
                }
            }
        }
    }
}
