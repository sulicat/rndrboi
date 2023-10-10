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

	struct QueFamilyInfo
	{
	    std::vector<int> graphics_family_indices;
	    std::vector<int> present_family_indices;
	    bool supports_graphics;
	    bool supports_present;
	};


	//--------------------------------------------------------------------------------

	void init_default();
	void setup_debug_cb();
	void update_physical_device_list();
        VkPhysicalDevice choose_device_auto();
	void create_surface();
	bool check_queue_families( VkPhysicalDevice dev );
	bool check_dev_extensions( VkPhysicalDevice dev );
	void create_logical_device( VkPhysicalDevice dev );
	void cleanup();
	QueFamilyInfo que_family_info( VkPhysicalDevice dev );

	static VulkanAPI* Instance();

    private:

	//--------------------------------------------------------------------------------

	const std::vector<const char*> validation_layers = {
	    "VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> added_extensions = {
	    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	const std::vector<const char*> device_extensions = {
	    VK_KHR_SWAPCHAIN_EXTENSION_NAME
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
	std::vector<VkPhysicalDevice> devices;

	VkPhysicalDevice selected_physical_device = VK_NULL_HANDLE;

	VkDevice device;
	VkQueue device_graphics_queue;
	VkSurfaceKHR surface;


	static VulkanAPI* singleton_instance;

    };

}

#endif
