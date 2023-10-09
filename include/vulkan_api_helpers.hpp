#ifndef _VULKAN_API_HELPERS_H_
#define _VULKAN_API_HELPERS_H_

#include <vulkan/vulkan.h>

VkResult CreateDebugUtilsMessengerEXT( VkInstance instance,
				       const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
				       const VkAllocationCallbacks* pAllocator,
				       VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT( VkInstance instance,
				    VkDebugUtilsMessengerEXT debugMessenger,
				    const VkAllocationCallbacks* pAllocator );

#endif

