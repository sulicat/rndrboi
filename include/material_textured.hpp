#pragma once

#include "material_base.hpp"
#include "vulkan_wrappers/vulkan_device.hpp"

namespace rndrboi
{

    class MaterialTextured : public MaterialBase
    {
    public:
        MaterialTextured();
        void set_diffuse( std::string path );

        VulkanTexture diffuse_texture;
        bool has_diffuse = false;
    };

};
