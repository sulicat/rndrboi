#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace rndrboi
{
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec4 color;
        glm::vec3 normal;
        glm::vec2 uv;

        static uint32_t offset_pos();
        static uint32_t offset_color();
        static uint32_t offset_normal();
        static uint32_t offset_uv();
        static uint32_t stride();
        static VkVertexInputBindingDescription binding_description();

    };
}
