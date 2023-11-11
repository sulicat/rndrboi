#pragma once

#include "vulkan_wrappers/vulkan_device.hpp"
#include "vulkan_wrappers/vulkan_descriptor_manager.hpp"

namespace rndrboi
{

    class MaterialTextured
    {
    public:
        MaterialTextured();
        void set_diffuse( std::string path );


        DescriptorManager descriptor_manager;
        bool needs_update = true;
        VulkanTexture diffuse_texture;
        bool has_diffuse = false;

        void clean();
    };

};
