#pragma once

#include <vector>
#include "vulkan_types.hpp"
#include <vulkan/vulkan.hpp>

namespace rndrboi
{

    class Swapchain{
    public:

	Swapchain();
	void create( VulkanDevice& dev );
	void clean();

	int size();
	int width();
	int height();

	VkSwapchainKHR			 swapchain;
	std::vector<VkImage>		 images;
	std::vector<VkImageView>	 image_views;
	VkExtent2D			 image_extent;
	VkFormat			 image_format;
	bool				 complete = false;

    private:
	VkSurfaceFormatKHR	 get_preferred_format( std::vector<VkSurfaceFormatKHR> format_in );
	VkPresentModeKHR	 get_preferred_mode( std::vector<VkPresentModeKHR> present_mode_in );
	VkExtent2D		 get_preferred_extent( VkSurfaceCapabilitiesKHR capabilities );
	std::vector<VkImage>	 get_swapchain_images( VulkanDevice& dev, VkSwapchainKHR sc );
	void			 create_image_views( VulkanDevice& dev );

	VulkanDevice* dev_internal;
    };


    std::ostream& operator<<(std::ostream&, Swapchain&);

}
