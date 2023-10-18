#include "vulkan_wrappers/vulkan_swapchain.hpp"

#include "window.hpp"
#include "config.hpp"

#include <set>
#include <algorithm>
#include <limits>
#include <iostream>


#define OK_PRINT (A_YELLOW "[VULKAN SWAPCHAIN] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN SWAPCHAIN] " A_RESET)

using namespace rndrboi;

std::ostream& rndrboi::operator<<(std::ostream& os, Swapchain& chain)
{
    os << "Swapchain: " << chain.width() << "x" << chain.height() << "  "
       << "Size: " << chain.size() << " "
       << "Format: " << chain.image_format << " "
       << "PresentMode: " << chain.present_mode << " "
       << "complete: " << chain.complete << "\n";
    return os;
}


Swapchain::Swapchain()
{
    complete = false;
}

int Swapchain::size()
{
    return image_views.size();
}

int Swapchain::width()
{
    return image_extent.width;
}

int Swapchain::height()
{
    return image_extent.height;
}

void Swapchain::create( VulkanDevice& dev )
{
    VkSurfaceFormatKHR chosen_surface_format	= get_preferred_format( dev.swapchain_info.formats );
    VkPresentModeKHR chosen_present_mode	= get_preferred_mode( dev.swapchain_info.present_modes );
    VkExtent2D chosen_extent			= get_preferred_extent( dev.swapchain_info.capabilities );
    uint32_t image_count			= dev.swapchain_info.capabilities.minImageCount + 1;

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType			= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface			= dev.surface;
    create_info.minImageCount		= image_count;
    create_info.imageFormat		= chosen_surface_format.format;
    create_info.imageColorSpace		= chosen_surface_format.colorSpace;
    create_info.imageExtent		= chosen_extent;
    create_info.imageArrayLayers	= 1;
    create_info.imageUsage		= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


    if( dev.queue_fam_info.graphics_family_index == dev.queue_fam_info.present_family_index )
    {
	create_info.imageSharingMode		= VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount	= 0;
	create_info.pQueueFamilyIndices		= nullptr;
    }
    else
    {
	std::cout << BAD_PRINT << "ERROR Unhandled case of concurrent que fams\n";
    }

    create_info.preTransform	= dev.swapchain_info.capabilities.currentTransform;
    create_info.compositeAlpha	= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode	= chosen_present_mode;
    create_info.clipped		= VK_FALSE;
    create_info.oldSwapchain	= VK_NULL_HANDLE;

    VkResult stat = vkCreateSwapchainKHR( dev.logical_device,
					  &create_info,
					  nullptr,
					  &swapchain );

    if( debug_print )
    {
	if( stat == VK_SUCCESS )
	    std::cout << OK_PRINT << "Created Swapchain\n";
	else
	    std::cout << BAD_PRINT << "ERROR failed to create swapchain\n";
    }

    images = get_swapchain_images( dev, swapchain );
    image_extent = chosen_extent;
    image_format = chosen_surface_format.format;
    present_mode = chosen_present_mode;

    create_image_views( dev );

    dev_internal = &dev;
    complete = true;
}


void Swapchain::clean()
{
    for( auto im : image_views )
	vkDestroyImageView( dev_internal->logical_device, im, nullptr );

    vkDestroySwapchainKHR(dev_internal->logical_device, swapchain, nullptr);

    if( debug_print )
	std::cout << OK_PRINT << "Cleaned swapchain\n";
}

std::vector<VkImage> Swapchain::get_swapchain_images( VulkanDevice& dev, VkSwapchainKHR sc )
{
    std::vector<VkImage> images;
    uint32_t image_count;
    vkGetSwapchainImagesKHR(dev.logical_device, sc, &image_count, nullptr);
    images.resize(image_count);
    vkGetSwapchainImagesKHR(dev.logical_device, sc, &image_count, images.data());

    return std::move(images);
}

VkSurfaceFormatKHR Swapchain::get_preferred_format( std::vector<VkSurfaceFormatKHR> format_in )
{
    for( auto f : format_in )
    {
	if( f.format == VK_FORMAT_B8G8R8A8_SRGB && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
	{
	    return f;
	}
    }
    return format_in[0];
}

VkPresentModeKHR Swapchain::get_preferred_mode( std::vector<VkPresentModeKHR> present_modes_in )
{
    for( auto pm : present_modes_in )
    {
	if( pm == VK_PRESENT_MODE_MAILBOX_KHR )
	{
	    if( debug_print )
		std::cout << "IDEAL PRESENT MODE";
	    return pm;
	}
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Swapchain::get_preferred_extent( VkSurfaceCapabilitiesKHR capabilities )
{

    if( Config::Instance()->window_manager == Config::WINDOW_MANAGER::GLFW )
    {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
	    return capabilities.currentExtent;
	}
	else
	{

	    rndrboi::WindowGLFW* glfw_window = dynamic_cast<rndrboi::WindowGLFW*>(Window::Instance()->get().get());

	    int width, height;
	    glfwGetFramebufferSize(glfw_window->window, &width, &height);
	    VkExtent2D actualExtent = {
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height)
	    };

	    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
	    return actualExtent;
	}
    }

    return capabilities.currentExtent;

}


void Swapchain::create_image_views( VulkanDevice& dev )
{
    image_views.resize( images.size() );

    for( int i = 0; i < images.size(); i++ )
    {
	VkImageViewCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	create_info.image = images[i];
	create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	create_info.format = image_format;

        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	create_info.subresourceRange.baseMipLevel = 0;
	create_info.subresourceRange.levelCount = 1;
	create_info.subresourceRange.baseArrayLayer = 0;
	create_info.subresourceRange.layerCount = 1;

	VkResult res = vkCreateImageView( dev.logical_device, &create_info, nullptr, &image_views[i]);

	if( debug_print )
	{
	    if( res == VK_SUCCESS )
		std::cout << OK_PRINT << "Created image view: " << i << "\n";
	    else
		std::cout << BAD_PRINT << "ERROR failed to create image view" << i << "\n";
	}
    }
}

std::pair<uint32_t, SWAPCHAIN_STATUS> Swapchain::acquire_next_image( Semaphore& sem )
{
    uint32_t index;
    SWAPCHAIN_STATUS status = OK;


    VkResult res = vkAcquireNextImageKHR( dev_internal->logical_device,
					  swapchain,
					  UINT64_MAX,
					  sem.vk_sem,
					  VK_NULL_HANDLE, &index);

    if( res == VK_ERROR_OUT_OF_DATE_KHR )
	status = OUT_OF_DATE;

    if( res == VK_SUBOPTIMAL_KHR )
	status = SUBOPTIMAL;

    if( res == VK_SUCCESS )
	status = OK;

    return std::make_pair(index, status);
}
