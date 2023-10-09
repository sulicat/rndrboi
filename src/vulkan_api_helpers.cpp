#include "vulkan_api_helpers.hpp"

#include "utils.hpp"


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


VKAPI_ATTR VkBool32 VKAPI_CALL debug_cb( VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
					 VkDebugUtilsMessageTypeFlagsEXT message_type,
					 const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
					 void* pUserData)
{


    if( message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT )
    {
	return VK_FALSE;
    }

    std::cout << A_RED << "[VULKAN type: 0x" << std::hex <<  (int)message_type << " sev: 0x" << std::hex << message_severity << "] " << A_RESET
	      << pCallbackData->pMessage
	      << "\n";

    return VK_FALSE;
}
