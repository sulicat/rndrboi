#pragma once

#include "vulkan_wrappers/vulkan_types.hpp"
#include <vulkan/vulkan.hpp>
#include <vector>

namespace rndrboi
{

    class Semaphore
    {
    public:
    Semaphore();
    void create( VulkanDevice& device );
    void clean();

    VkSemaphore vk_sem;

    private:
    VulkanDevice* internal_dev;
    };


    class Fence
    {
    public:
    Fence();
    void create( VulkanDevice& device, bool is_signaled = false );
    void clean();
    void wait( uint64_t timeout = UINT64_MAX );
    void reset();

    VkFence vk_fence;

    private:
    VulkanDevice* internal_dev;
    };

}


