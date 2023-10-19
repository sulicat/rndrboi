#pragma once

#include "vulkan_memory_allocator/vk_mem_alloc.h"

#include "vulkan_wrappers/vulkan_types.hpp"

namespace rndrboi
{
    //----------------------------------------------------------------------------------------------------

    class Buffer
    {
    public:

	Buffer(){}
	bool is_good() { return is_set; }
	uint32_t get_size() { return size; }

	VkBuffer buffer;
	VmaAllocation allocation;

    private:

	Buffer( VkBuffer buffer_in, VmaAllocation alloc_in, uint32_t size_in )
	{
	    set( buffer_in, alloc_in, size_in );
	}

	void set( VkBuffer buffer_in, VmaAllocation alloc_in, uint32_t size_in )
	{
	    buffer = buffer_in;
	    allocation = alloc_in;
	    is_set = true;
	    size = size_in;
	}

	bool is_set		= false;
	void* mapped_memory	= nullptr;
	bool is_mapped		= false;
	uint32_t size = 0;

	// only yhr buffer manager can crate a Buffer that is markeg "good"
	friend class BufferManager;
    };

    //----------------------------------------------------------------------------------------------------

    struct StagingBufferPair{
	Buffer staging_buffer;
	Buffer device_buffer;
    };

    //----------------------------------------------------------------------------------------------------

    // singleton class, only 1 guy will ever manager the buffers
    class BufferManager
    {
    public:
	static BufferManager* Instance();

	void init( VulkanDevice& dev );
	Buffer get_buffer( BufferSettings settings );
	StagingBufferPair get_staing_buffer_pair( int size, VkBufferUsageFlags usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT  );
	void* get_mapped_memory( Buffer& buff );

	void clean_buffer( Buffer& buff );
	void clean();

    private:
	BufferManager(){};
	VulkanDevice* internal_device;
	bool is_initialized = false;
	static BufferManager* instance;

    };

};
