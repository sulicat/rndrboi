#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include <vector>
#include "vulkan_wrappers/vulkan_vertex.hpp"


namespace rndrboi
{

    class Mesh
    {
    public:
    Mesh();

    int use_indices = false;

    std::vector<Vertex> vertex_data;
    };

}
