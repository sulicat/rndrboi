#ifndef _VULKAN_API_H_
#define _VULKAN_API_H_

#include "window.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "vulkan_types.hpp"
#include "vulkan_init.hpp"
#include "vulkan_swapchain.hpp"
#include <vulkan/vulkan.h>



namespace rndrboi
{

    class VulkanAPI
    {
    public:

	void init_default();
	void cleanup();

	static VulkanAPI* Instance();

    private:

	VulkanAPI(){}
	~VulkanAPI(){ cleanup(); }

	rndrboi::VulkanDevice device_data;
	rndrboi::Swapchain swapchain;

	static VulkanAPI* singleton_instance;

    };

}

#endif
