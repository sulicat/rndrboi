#pragma once


#include <vector>
#include "vulkan_types.hpp"
#include <vulkan/vulkan.hpp>

namespace rndrboi
{
    class GraphicsPipeline
    {
    public:
	GraphicsPipeline();
	void create( VulkanDevice& dev );
    };

};
