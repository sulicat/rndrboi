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

	//--------------------------------------------------------------------------------

	void init_default();
	void setup_debug_cb();
	void update_physical_devices();
	void choose_device_auto();
	bool check_queue_families( VkPhysicalDevice dev );
	void cleanup();

	static VulkanAPI* Instance();

    private:

	//--------------------------------------------------------------------------------

	const std::vector<const char*> validation_layers = {
	    "VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> added_extensions = {
	    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	//--------------------------------------------------------------------------------

	VulkanAPI(){}
	~VulkanAPI(){ cleanup(); }

	void create_vk_instance();
	std::vector<VkExtensionProperties> get_supported_extentions();
	std::vector<VkLayerProperties> get_supported_layers();
	bool check_layer_support();
	bool check_extension_support();

	//--------------------------------------------------------------------------------

	VkInstance instance;
	std::vector<VkExtensionProperties> supported_extensions;
	std::vector<VkLayerProperties> supported_layers;
	std::vector<const char*> extensions;
	VkDebugUtilsMessengerEXT debug_messenger;
	VkPhysicalDevice selected_device = VK_NULL_HANDLE;
	std::vector<VkPhysicalDevice> devices;

	static VulkanAPI* singleton_instance;

    };

}

#endif
