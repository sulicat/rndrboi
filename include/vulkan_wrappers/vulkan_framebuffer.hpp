#pragma once

#include <vector>
#include "vulkan_wrappers/vulkan_swapchain.hpp"
#include "vulkan_wrappers/vulkan_render_pass.hpp"

#include "vulkan_wrappers/vulkan_types.hpp"
#include <vulkan/vulkan.hpp>

namespace rndrboi
{
    class Framebuffer
    {
    public:
        Framebuffer();
        void create( VulkanDevice& dev, Swapchain& swapchain, RenderPass& render_pass );
        void clean();

        std::vector<VkFramebuffer> swapchain_framebuffers;

    private:
        VulkanDevice* internal_dev;
        Swapchain* internal_swapchain;
        RenderPass* internal_render_pass;
    };

};
