#pragma once

#include <vector>
#include "vulkan_wrappers/vulkan_types.hpp"
#include <vulkan/vulkan.hpp>

namespace rndrboi
{
    class RenderPass
    {
    public:
        RenderPass();
        void create( VulkanDevice& dev, RenderPassSettings settings );
        void clean();

        VkRenderPass render_pass;

    private:
        RenderPassSettings settings;
        VulkanDevice* dev_internal;

    };

};
