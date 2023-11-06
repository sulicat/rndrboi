#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "vulkan_wrappers/vulkan_buffer_manager.hpp"
#include "vulkan_wrappers/vulkan_types.hpp"


namespace rndrboi
{
    class VulkanTexture
    {
    public:
        VulkanTexture();
        void create( VulkanDevice& dev );
        void load( std::string path );
        void from_data( char* data, int w_in, int h_in, int ch_in );
        void clean();

        int width = 0;
        int height = 0;
        int channels = 0;
        int image_size = 0;

        rndrboi::Buffer* staging_buffer;
        void* staging_buffer_ptr = NULL;

        rndrboi::ImageBuffer* image;
        VkImageView image_view;

    private:
        bool init_success = false;
        VulkanDevice* internal_device = NULL;
    };
};
