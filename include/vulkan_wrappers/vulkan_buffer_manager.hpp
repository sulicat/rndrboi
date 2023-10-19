#pragma once

#include "vulkan_memory_allocator/vk_mem_alloc.h"

#include "vulkan_wrappers/vulkan_types.hpp"

namespace rndrboi
{
    // singleton class, only 1 guy will ever manager the buffers
    class BufferManager
    {
    public:
	static BufferManager* Instance();
	void init( VulkanDevice& dev );
	std::pair<VkBuffer, VmaAllocation> get_buffer( BufferSettings settings );
	void clean_buffer( VkBuffer buffer, VmaAllocation alloc );
	void clean();

    private:
	BufferManager(){};

	VulkanDevice* internal_device;
	bool is_initialized = false;
	static BufferManager* instance;

    };

};
