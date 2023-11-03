#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "vulkan_wrappers/vulkan_types.hpp"


namespace rndrboi
{
    class VulkanTexture
    {
	VulkanTexture();
	void create( VulkanDevice& dev );
    };
};
