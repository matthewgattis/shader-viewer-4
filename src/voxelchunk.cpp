#include "voxelchunk.hpp"

#include <glm/gtc/noise.hpp>

VoxelChunk::VoxelChunk(const glm::ivec2 position)
{
    voxels_.reserve(SCALE.x * SCALE.y * SCALE.z);
}

