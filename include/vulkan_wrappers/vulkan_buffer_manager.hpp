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

    Buffer( uint32_t size_in )
    {
        set( size_in );
    }

    void set( uint32_t size_in )
    {
        is_set = true;
        size = size_in;
    }

    bool is_set     = false;
    void* mapped_memory = nullptr;
    bool is_mapped      = false;
    uint32_t size = 0;

    // only yhr buffer manager can crate a Buffer that is markeg "good"
    friend class BufferManager;
    };

    //----------------------------------------------------------------------------------------------------

    class ImageBuffer
    {
    public:
    bool is_good() { return is_set; }

    VkImage image;
    VmaAllocation allocation;

    private:

    ImageBuffer()
    {
        set();
    }

    void set()
    {
        is_set = true;
    }

    bool is_set     = false;
    void* mapped_memory = nullptr;
    bool is_mapped      = false;

    // only yhr buffer manager can crate a Buffer that is markeg "good"
    friend class BufferManager;
    };

    //----------------------------------------------------------------------------------------------------

    // singleton class, only 1 guy will ever manager the buffers
    class BufferManager
    {
    public:
    static BufferManager* Instance();

    void init( VulkanDevice& dev );
    Buffer* get_buffer( BufferSettings settings );
    ImageBuffer* get_image_buffer( ImageBufferSettings settings );

    void* get_mapped_memory( Buffer& buff );
    void* get_mapped_memory( ImageBuffer& buff );

    void clean_buffer( Buffer& buff );
    void clean_buffer( ImageBuffer& buff );
    void clean();

    private:
    BufferManager(){};
    VulkanDevice* internal_device;
    bool is_initialized = false;
    static BufferManager* instance;

    std::vector<Buffer*> all_buffers;
    std::vector<ImageBuffer*> all_image_buffers;

    };

};
