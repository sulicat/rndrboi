#ifndef _VULKAN_API_H_
#define _VULKAN_API_H_

#include "window.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "vulkan_types.hpp"
#include "vulkan_init.hpp"
#include <vulkan/vulkan.h>



namespace rndrboi
{

    class VulkanAPI
    {
    public:

	void init_default();

	// swap chain
        VkSurfaceFormatKHR get_preferred_format( std::vector<VkSurfaceFormatKHR> format_in );
        VkPresentModeKHR get_preferred_mode( std::vector<VkPresentModeKHR> present_mode_in );
        VkExtent2D get_preferred_extent( VkSurfaceCapabilitiesKHR capabilities );
	void create_swapchain( VkPhysicalDevice dev );
	std::vector<VkImage> get_swapchain_images( VkSwapchainKHR sc );
	void create_image_views();

	void cleanup();


	static VulkanAPI* Instance();

    private:

	VulkanAPI(){}
	~VulkanAPI(){ cleanup(); }

	//--------------------------------------------------------------------------------

	rndrboi::VulkanDevice device_data;

	VkSwapchainKHR swap_chain;
	std::vector<VkImage> swapchain_images;
	std::vector<VkImageView> swapchain_image_views;
	VkExtent2D swapchain_image_extent;
        VkFormat swapchain_image_format;

	static VulkanAPI* singleton_instance;

    };

}

#endif
