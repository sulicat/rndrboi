#include "vulkan_api_helpers.hpp"


VkResult CreateDebugUtilsMessengerEXT( VkInstance instance,
				       const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
				       const VkAllocationCallbacks* pAllocator,
				       VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr)
	return func(instance, pCreateInfo, pAllocator, pDebugMessenger);

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}


void DestroyDebugUtilsMessengerEXT( VkInstance instance,
				    VkDebugUtilsMessengerEXT debugMessenger,
				    const VkAllocationCallbacks* pAllocator )
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != nullptr)
	func(instance, debugMessenger, pAllocator);
}
