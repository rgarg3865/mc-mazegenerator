//
#ifndef TERRAFORM
#define TERRAFORM

#include </usr/local/include/mcpp/mcpp.h>

#include <algorithm>
#include <chrono>
#include <thread>

#include "Queue.h"

struct BlockData {
    mcpp::Coordinate coord{};
    mcpp::BlockType type{};
};

class Terraform
{
   public:
    Terraform(std::shared_ptr<mcpp::MinecraftConnection> mc, bool fastmode);
    void terraformedAreaRestore();
    void terraformArea(mcpp::Coordinate, int, int);

   private:
    std::shared_ptr<mcpp::MinecraftConnection> mc;
    int length;
    int width;
    mcpp::Coordinate basePoint;
    bool isFastmode;
    mcpp::BlockType air;
    mcpp::Coordinate corner{};
    mcpp::Coordinate oppLowerCorner{};
    mcpp::Coordinate oppUpperCorner{};
    Queue<BlockData> blocksInUpperChunk{};
    mcpp::HeightMap terrainHeights;

    void fillChunk(mcpp::Coordinate, mcpp::Coordinate, mcpp::BlockType&);
    void terraformLowerChunk();
    void storeBlocksInChunk(mcpp::Coordinate& corner, mcpp::Coordinate& oppUpperCorner);
    void setCornerCoordinates();
    void restoreFromHeight(int height);
    mcpp::HeightMap getHeightMap();
};

#endif
