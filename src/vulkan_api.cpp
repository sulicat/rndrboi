#include "vulkan_api.hpp"
#include "vulkan_api_helpers.hpp"

#include <set>
#include <algorithm>
#include <limits>

using namespace rndrboi;

VulkanAPI* VulkanAPI::singleton_instance = NULL;

VulkanAPI* VulkanAPI::Instance()
{
    if (!singleton_instance)
    {
	singleton_instance = new VulkanAPI;
    }
    return singleton_instance;
}

//----------------------------------------------------------------------------------------------------

void VulkanAPI::init_default()
{
    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "init default\n";

    rndrboi::VulkanDevicePreferences dev_preferences{};
    dev_preferences.graphics		= true;
    dev_preferences.present		= true;
    dev_preferences.limit_one_queue	= true;
    dev_preferences.debug		= true;
    dev_preferences.print_info		= true;

    device_data = VulkanDeviceInit::init( dev_preferences );

    //create_swapchain( device_data );
    //create_image_views();
}

void VulkanAPI::create_swapchain( VkPhysicalDevice dev  )
{
    /*
    SwapChainSupportInfo swap_chain_info = get_swapchain_support_info( dev );
    VkSurfaceFormatKHR chosen_surface_format = get_preferred_format( swap_chain_info.formats );
    VkPresentModeKHR chosen_present_mode = get_preferred_mode( swap_chain_info.present_modes );
    VkExtent2D chosen_extent = get_preferred_extent( swap_chain_info.capabilities );

    uint32_t image_count = swap_chain_info.capabilities.minImageCount + 1;

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = chosen_surface_format.format;
    create_info.imageColorSpace = chosen_surface_format.colorSpace;
    create_info.imageExtent = chosen_extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueFamilyInfo q_fam_info = que_family_info( dev );

    if( q_fam_info.graphics_family_indices[0] == q_fam_info.present_family_indices[0] )
    {
	create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
	create_info.pQueueFamilyIndices = nullptr;
    }
    else
    {
	std::cout << A_RED << "[VAPI] " << A_RESET << "ERROR Unhandled case of concurrent que fams\n";
    }

    create_info.preTransform = swap_chain_info.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = chosen_present_mode;
    create_info.clipped = VK_FALSE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    VkResult stat = vkCreateSwapchainKHR( device,
					  &create_info,
					  nullptr,
					  &swap_chain );


    if( stat == VK_SUCCESS )
	std::cout << A_YELLOW << "[VAPI] " << A_RESET << "Created Swapchain\n";
    else
	std::cout << A_RED << "[VAPI] " << A_RESET << "ERROR failed to create swapchain\n";

    swapchain_images = get_swapchain_images( swap_chain );
    swapchain_image_extent = chosen_extent;
    swapchain_image_format = chosen_surface_format.format;
    */
}

void VulkanAPI::create_image_views()
{
    /*
    swapchain_image_views.resize( swapchain_images.size() );

    for( int i = 0; i < swapchain_images.size(); i++ )
    {
	VkImageViewCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	create_info.image = swapchain_images[i];
	create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	create_info.format = swapchain_image_format;

        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	create_info.subresourceRange.baseMipLevel = 0;
	create_info.subresourceRange.levelCount = 1;
	create_info.subresourceRange.baseArrayLayer = 0;
	create_info.subresourceRange.layerCount = 1;

	VkResult res = vkCreateImageView( device, &create_info, nullptr, &swapchain_image_views[i]);

	if( res == VK_SUCCESS )
	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "Created image view: " << i << "\n";
	else
	    std::cout << A_RED << "[VAPI] " << A_RESET << "ERROR failed to create image view" << i << "\n";

    }
    */
}


std::vector<VkImage> VulkanAPI::get_swapchain_images( VkSwapchainKHR sc )
{

    std::vector<VkImage> images;
    /*
    uint32_t image_count;
    vkGetSwapchainImagesKHR(device, sc, &image_count, nullptr);
    images.resize(image_count);
    vkGetSwapchainImagesKHR(device, sc, &image_count, images.data());
    */

    return std::move(images);
}

VkSurfaceFormatKHR VulkanAPI::get_preferred_format( std::vector<VkSurfaceFormatKHR> format_in )
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

VkPresentModeKHR VulkanAPI::get_preferred_mode( std::vector<VkPresentModeKHR> present_modes_in )
{
    for( auto pm : present_modes_in )
    {
	if( pm == VK_PRESENT_MODE_MAILBOX_KHR )
	{
	    std::cout << "IDEAL PRESENT MODE";
	    return pm;
	}
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanAPI::get_preferred_extent( VkSurfaceCapabilitiesKHR capabilities )
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

void VulkanAPI::cleanup()
{
    VulkanDeviceInit::clean( device_data );

    /*
    for( auto im : swapchain_image_views )
	vkDestroyImageView( device, im, nullptr );

    vkDestroySwapchainKHR(device, swap_chain, nullptr);
    */
}
