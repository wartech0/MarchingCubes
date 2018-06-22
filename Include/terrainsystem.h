#ifndef TERRAINSYSTEM_H
#define TERRAINSYSTEM_H

#include "terrainchunk.h"

class TerrainSystem
{
public:
    TerrainSystem();

    //TerrainChunk GetChunkByView(ViewFrustrum view);

private:
    TerrainChunk _chunks;
};

#endif // TERRAINSYSTEM_H
