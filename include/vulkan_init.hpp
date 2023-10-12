#pragma once

#include "window.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "vulkan_types.hpp"
#include <vulkan/vulkan.h>


namespace rndrboi
{
    namespace VulkanDeviceInit
    {
	VulkanDevice init( const VulkanDevicePreferences& pref );
	bool create_instance( VulkanDevice& dev, const VulkanDevicePreferences& pref );
	bool clean( VulkanDevice& dev );
	void setup_debug_cb( VulkanDevice& dev, const VulkanDevicePreferences& pref );
	void create_surface( VulkanDevice& dev, const VulkanDevicePreferences& pref );
	void choose_physical_device( VulkanDevice& dev, const VulkanDevicePreferences& pref );
	void create_logical_device( VulkanDevice& dev, const VulkanDevicePreferences& pref );

	std::vector<VkExtensionProperties> get_supported_extentions();
	std::vector<VkLayerProperties> get_supported_layers();
	bool check_layer_support( std::vector<const char*> layers );
	bool check_extension_support( std::vector<const char*> extensions );
	SwapChainSupportInfo get_swapchain_support_info( VkPhysicalDevice& dev, VkSurfaceKHR& surface );
	QueFamilyInfo que_family_info( VulkanDevice& dev );
    };


    std::ostream& operator<<(std::ostream&, SwapChainSupportInfo&);

};
