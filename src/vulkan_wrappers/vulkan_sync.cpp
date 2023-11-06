#include "utils.hpp"
#include "vulkan_wrappers/vulkan_sync.hpp"

using namespace rndrboi;

#define OK_PRINT (A_YELLOW "[VULKAN SYNC] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN SYNC] " A_RESET)

//----------------------------------------------------------------------------------------------------

Semaphore::Semaphore()
{
}

void Semaphore::create( VulkanDevice& dev )
{
    internal_dev = &dev;

    VkSemaphoreCreateInfo sem_create_info{};
    sem_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkResult res = vkCreateSemaphore( dev.logical_device,
                                      &sem_create_info,
                                      nullptr,
                                      &vk_sem );

    if( res != VK_SUCCESS )
        std::cout << BAD_PRINT << "ERROR could not create semaphore\n";
}

void Semaphore::clean()
{
    vkDestroySemaphore( internal_dev->logical_device, vk_sem, nullptr );
}

//----------------------------------------------------------------------------------------------------

Fence::Fence()
{
}

void Fence::create( VulkanDevice& dev, bool is_signaled )
{
    internal_dev = &dev;

    VkFenceCreateInfo fence_create_info{};
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    if (is_signaled)
        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkResult res = vkCreateFence( dev.logical_device,
                                  &fence_create_info,
                                  nullptr,
                                  &vk_fence );

    if( res != VK_SUCCESS )
        std::cout << BAD_PRINT << "ERROR could not create semaphore\n";
}

void Fence::clean()
{
    vkDestroyFence( internal_dev->logical_device, vk_fence, nullptr );
}

void Fence::wait( uint64_t timeout )
{
    vkWaitForFences( internal_dev->logical_device, 1, &vk_fence, VK_TRUE, timeout );
}

void Fence::reset()
{
    vkResetFences( internal_dev->logical_device, 1, &vk_fence );
}
