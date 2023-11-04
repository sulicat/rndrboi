#include "vulkan_wrappers/vulkan_texture.hpp"
#include "utils.hpp"

// for now this loads the file, in future the asset manager does
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define OK_PRINT (A_YELLOW "[VULKAN TEXTURE] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN TEXTURE] " A_RESET)


using namespace rndrboi;

VulkanTexture::VulkanTexture(){}

void VulkanTexture::create( VulkanDevice& device )
{
    internal_device = &device;
    init_success = true;
}

void VulkanTexture::load( std::string path )
{
    if( internal_device == NULL || init_success == false )
    {
	std::cout << BAD_PRINT << "Texture not initialized properly, cannot load\n";
	return;
    }

    int stb_width, stb_height, stb_channels;
    stbi_uc* pixels = stbi_load(path.c_str(),
				&stb_width, &stb_height, &stb_channels,
				STBI_rgb_alpha);

    std::cout << OK_PRINT << stb_width << "x" << stb_height << " " << stb_channels << " channels\n";

    if( !pixels )
    {
	std::cout << BAD_PRINT << "Failed to load: " << path << "\n";
	return;
    }

    from_data( (char*)pixels, stb_width, stb_height, 4 ); // forced alpha channel via stbi load

    stbi_image_free(pixels);
}


void VulkanTexture::from_data( char* data, int w_in, int h_in, int ch_in )
{

    width = w_in;
    height = h_in;
    channels = ch_in;
    image_size = width*height*channels;

    staging_buffer = BufferManager::Instance()->get_buffer({ .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT });
    staging_buffer_ptr = BufferManager::Instance()->get_mapped_memory( *staging_buffer );
    memcpy(staging_buffer_ptr, data, image_size );

    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent.width = (uint32_t)width;
    image_info.extent.height = (uint32_t)height;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = VK_FORMAT_R8G8B8A8_SRGB;
    image_info.tiling = VK_IMAGE_TILING_LINEAR;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.flags = 0;

    image = BufferManager::Instance()->get_image_buffer({ .create_info = image_info });

    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = image->image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = VK_FORMAT_R8G8B8A8_SRGB;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    VkResult res = vkCreateImageView( internal_device->logical_device,
				      &view_info,
				      nullptr,
				      &image_view );

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "Could not create image view\n";
}

void VulkanTexture::clean()
{
    vkDestroyImageView( internal_device->logical_device,
			image_view,
			nullptr );
}
