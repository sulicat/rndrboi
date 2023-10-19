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
    allocator_create_info.physicalDevice	= dev.physical_device;
    allocator_create_info.device		= dev.logical_device;
    allocator_create_info.instance		= dev.instance;
    allocator_create_info.pVulkanFunctions	= NULL;

    VkResult res = vmaCreateAllocator( &allocator_create_info, &allocator );
    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not create memory allocator\n";

    is_initialized = true;
}

std::pair<VkBuffer, VmaAllocation> BufferManager::get_buffer( BufferSettings settings )
{

    if( !is_initialized )
	std::cout << BAD_PRINT << "ERROR Need to call init( dev ) on buffer manager\n";

    VkBufferCreateInfo buffer_create_info{};
    buffer_create_info.sType		= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size		= settings.buffer_size;
    buffer_create_info.usage		= settings.usage;
    buffer_create_info.sharingMode	= settings.sharing_mode;

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

    VkBuffer buffer;
    VmaAllocation allocation;
    VkResult res = vmaCreateBuffer(allocator, &buffer_create_info, &alloc_info, &buffer, &allocation, nullptr);
    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not create buffer of size: " << buffer_create_info.size << "\n";

    return std::move( std::make_pair(buffer, allocation) );
}

void BufferManager::clean_buffer( VkBuffer buffer, VmaAllocation alloc )
{
    vmaDestroyBuffer(allocator, buffer, alloc);
}

void BufferManager::clean()
{
    vmaDestroyAllocator(allocator);
}


