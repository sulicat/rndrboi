#pragma once

#include "vulkan_wrappers/vulkan_descriptor_manager.hpp"

namespace rndrboi
{

    class MaterialBase
    {
    public:

        MaterialBase() = default;

        DescriptorManager descriptor_manager;
        bool needs_update = true;
    };

};

