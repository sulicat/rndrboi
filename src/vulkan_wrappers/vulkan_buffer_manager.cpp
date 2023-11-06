#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS VK_TRUE
#define VMA_DYNAMIC_VULKAN_FUNCTIONS VK_TRUE
#include "vulkan_memory_allocator/vk_mem_alloc.h"

#include "vulkan_wrappers/vulkan_buffer_manager.hpp"
#include "utils.hpp"

#define ATTENTION_PRINT (A_BLUE "[BUFFER MANAGER] " A_RESET)
#define OK_PRINT (A_YELLOW "[BUFFER MANAGER] " A_RESET)
#define BAD_PRINT (A_RED "[BUFFER MANAGER] " A_RESET)

using namespace rndrboi;

VmaAllocator allocator;
BufferManager* BufferManager::instance = NULL;


BufferManager* BufferManager::Instance()
{
    if (!instance){
    std::cout << OK_PRINT << "Created Buffer Manager\n";
    instance = new BufferManager;
    }
    return instance;
}

//----------------------------------------------------------------------------------------------------

void BufferManager::init( VulkanDevice& dev )
{
    internal_device = &dev;

    VmaAllocatorCreateInfo allocator_create_info = {};
    allocator_create_info.physicalDevice    = dev.physical_device;
    allocator_create_info.device        = dev.logical_device;
    allocator_create_info.instance      = dev.instance;
    allocator_create_info.pVulkanFunctions  = NULL;

    VkResult res = vmaCreateAllocator( &allocator_create_info, &allocator );
    if( res != VK_SUCCESS )
    std::cout << BAD_PRINT << "ERROR could not create memory allocator\n";

    is_initialized = true;
}

Buffer* BufferManager::get_buffer( BufferSettings settings )
{
    if( !is_initialized )
    std::cout << BAD_PRINT << "ERROR Need to call init( dev ) on buffer manager\n";

    VkBufferCreateInfo buffer_create_info{};
    buffer_create_info.sType        = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size     = settings.buffer_size;
    buffer_create_info.usage        = settings.usage;
    buffer_create_info.sharingMode  = settings.sharing_mode;

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
    alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT; // host accesible

    Buffer* buffer_out = new Buffer( settings.buffer_size );

    VkResult res = vmaCreateBuffer( allocator,
                    &buffer_create_info,
                    &alloc_info,
                    &buffer_out->buffer,
                    &buffer_out->allocation,
                    nullptr );
    if( res != VK_SUCCESS )
    std::cout << BAD_PRINT << "ERROR could not create buffer of size: " << buffer_create_info.size << "\n";

    all_buffers.push_back( buffer_out );

    return buffer_out;
}

ImageBuffer* BufferManager::get_image_buffer( ImageBufferSettings settings )
{
    if( !is_initialized )
    std::cout << BAD_PRINT << "ERROR Need to call init( dev ) on buffer manager\n";

    ImageBuffer* image_out = new ImageBuffer();

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
    alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT; // host accesible

    VkResult res = vmaCreateImage( allocator,
                   &settings.create_info,
                   &alloc_info,
                   &image_out->image,
                   &image_out->allocation,
                   nullptr );

    if( res != VK_SUCCESS )
    std::cout << BAD_PRINT << "ERROR could not create image\n";

    all_image_buffers.push_back( image_out );

    return image_out;
}


void* BufferManager::get_mapped_memory( Buffer& buff )
{
    if( !buff.is_good() )
    std::cout << BAD_PRINT << "ERROR buffer memory mapping failed. Buffer not initialized properly\n";

    if( !buff.is_mapped )
    {
    vmaMapMemory(allocator, buff.allocation, &buff.mapped_memory);
    buff.is_mapped = true;
    }

    return buff.mapped_memory;
}

void* BufferManager::get_mapped_memory( ImageBuffer& buff )
{
    if( !buff.is_good() )
    std::cout << BAD_PRINT << "ERROR image memory mapping failed. Image Buffer not initialized properly\n";

    if( !buff.is_mapped )
    {
    vmaMapMemory(allocator, buff.allocation, &buff.mapped_memory);
    buff.is_mapped = true;
    }

    return buff.mapped_memory;
}


void BufferManager::clean_buffer( Buffer& buff )
{
    if( buff.is_mapped )
    vmaUnmapMemory(allocator, buff.allocation);

    vmaDestroyBuffer(allocator, buff.buffer, buff.allocation);
}

void BufferManager::clean_buffer( ImageBuffer& buff )
{
    if( buff.is_mapped )
    vmaUnmapMemory(allocator, buff.allocation);

    vmaDestroyImage(allocator, buff.image, buff.allocation);
}

void BufferManager::clean()
{

    for( int i = 0; i < all_image_buffers.size(); i++ )
    clean_buffer( *all_image_buffers[i] );

    for( int i = 0; i < all_buffers.size(); i++ )
    clean_buffer( *all_buffers[i] );

    vmaDestroyAllocator(allocator);
}

