#ifndef _VULKAN_API_H_
#define _VULKAN_API_H_

#include "window.hpp"
#include "utils.hpp"
#include "config.hpp"
#include <vulkan/vulkan.h>

namespace rndrboi
{

    class VulkanAPI
    {
    public:

	void init_default();
	void setup_debug_cb();
	void cleanup();
	static VulkanAPI* Instance();

    private:

	const std::vector<const char*> validation_layers = {
	    "VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> added_extensions = {
	    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_cb( VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
							VkDebugUtilsMessageTypeFlagsEXT message_type,
							const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
							void* pUserData);


	VulkanAPI(){}
	~VulkanAPI(){ cleanup(); }
	void create_vk_instance();
	std::vector<VkExtensionProperties> get_supported_extentions();
	std::vector<VkLayerProperties> get_supported_layers();
	bool check_layer_support();
	bool check_extension_support();


	VkInstance instance;
	std::vector<VkExtensionProperties> supported_extensions;
	std::vector<VkLayerProperties> supported_layers;
	std::vector<const char*> extensions;
	VkDebugUtilsMessengerEXT debug_messenger;

	static VulkanAPI* singleton_instance;

    };

}

#endif
