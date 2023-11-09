#pragma once

#include "utils.hpp"

#include "vulkan_wrappers/vulkan_api_helpers.hpp"
#include "vulkan_wrappers/vulkan_types.hpp"
#include "vulkan_wrappers/vulkan_init.hpp"

namespace rndrboi
{

    class Device
    {
    public:

        static Device* Instance();
        void init();

        VulkanDevice device;

    private:

        Device(){}
        static Device* instance;

    };

};
