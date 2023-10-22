#include "vulkan_wrappers/vulkan_vertex.hpp"

using namespace rndrboi;


uint32_t Vertex::offset_pos()
{
    return offsetof(Vertex, pos);
}

uint32_t Vertex::offset_color()
{
    return offsetof(Vertex, color);
}

uint32_t Vertex::offset_normal()
{
    return offsetof(Vertex, normal);
}

uint32_t Vertex::offset_uv()
{
    return offsetof(Vertex, uv);
}

uint32_t Vertex::stride()
{
    return sizeof(Vertex);
}

VkVertexInputBindingDescription Vertex::binding_description()
{
    VkVertexInputBindingDescription description{};
    description.binding = 0;
    description.stride = Vertex::stride();
    description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return description;
}
