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


VKAPI_ATTR VkBool32 VKAPI_CALL debug_cb( VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                     VkDebugUtilsMessageTypeFlagsEXT message_type,
                     const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                     void* pUserData);

#endif

