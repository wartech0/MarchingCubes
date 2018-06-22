#ifndef CHUNKGENERATOR_H
#define CHUNKGENERATOR_H

#include <atomic>
#include <cstddef>
#include <thread>

#include "terrainchunk.h"

const size_t bufferSize = 1024;

class ChunkGenerator
{
public:
    ChunkGenerator();

    void SubmitJob(const TerrainChunk& chunk);

private:
    std::atomic<size_t> m_head;
    std::atomic<size_t> m_tail;

    TerrainChunk* m_buffer;

    size_t next(size_t current);

    std::thread m_workerThread;
};

#endif // CHUNKGENERATOR_H
