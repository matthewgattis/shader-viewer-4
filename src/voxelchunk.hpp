#pragma once

#include <vector>

#include <glm/glm.hpp>

class VoxelChunk : Renderable
{
public:
    VoxelChunk(const glm::ivec2 position);

private:
    std::vector<int> voxels_;

    static const glm::ivec3 SCALE;
};

