#pragma once

#include <vector>
#include <string>
#include <optional>
#include <vulkan/vulkan.h>

namespace rndrboi
{
    struct SwapChainSupportInfo
    {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> present_modes;
    };

    struct QueFamilyInfo
    {
	std::vector<int> graphics_family_indices;
	std::vector<int> present_family_indices;
	bool supports_graphics;
	bool supports_present;
    };

    struct VulkanDevice
    {
	VkInstance			 instance;
	VkPhysicalDevice		 physical_device	= VK_NULL_HANDLE;
	VkDevice			 logical_device		= VK_NULL_HANDLE;
	std::vector<int>		 graphics_family_indices;
	std::vector<int>		 present_family_indices;
	bool				 supports_graphics;
	bool				 supports_present;
	std::vector<const char*>         extensions;
	std::vector<const char*>         validation_layers;
	VkSurfaceKHR			 surface = NULL;
	VkDebugUtilsMessengerEXT	 debug_messenger;
	std::vector<VkPhysicalDevice>	 available_devices;
	SwapChainSupportInfo		 swapchain_info;
	QueFamilyInfo			 queue_fam_info;
    };

    struct VulkanDevicePreferences
    {
	bool graphics			= true;
	bool present			= true;
	bool dedicated_gpu		= true;
	bool limit_one_queue		= true;
	bool debug			= true;
	bool print_info			= true;
	bool geometry_shader		= false;
	bool tessellation_shader	= false;
	std::string app_name		= "rndrboiapp";
	std::string engine_name		= "rndrboi";
	int app_ver[3]			= {0,0,1};
	int engine_ver[3]		= {0,0,1};
    };


    struct GraphicsPipelineSettings
    {
	std::string vert_shader_path;
	std::string frag_shader_path;
	std::string vert_entrypoint = "main";
	std::string frag_entrypoint = "main";

	// add constants for every stage
	// look at pSpecializationInfo here: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPipelineShaderStageCreateInfo.html

	std::optional<std::string> tesselation_shader_path;
	std::optional<std::string> geometry_shader_path;
    };

};
