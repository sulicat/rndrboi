#pragma once

#include "vulkan_wrappers/vulkan_types.hpp"
#include "vulkan_wrappers/vulkan_sync.hpp"
#include <vulkan/vulkan.hpp>

namespace rndrboi
{

    class DescriptorSet
    {
    public:
	DescriptorSet();
	void create( VulkanDevice& dev, DescriptorSetSettings settings );

	VkDescriptorSetLayout descriptor_set_layout;

    private:
	VulkanDevice* internal_device;
    };


};
