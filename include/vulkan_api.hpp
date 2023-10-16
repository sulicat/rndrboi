#ifndef _VULKAN_API_H_
#define _VULKAN_API_H_

#include "window.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "vulkan_types.hpp"
#include "vulkan_init.hpp"
#include "vulkan_swapchain.hpp"
#include "vulkan_graphics_pipeline.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_framebuffer.hpp"
#include "vulkan_command_manager.hpp"
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
	rndrboi::RenderPass render_pass;
	rndrboi::GraphicsPipeline pipeline;
	rndrboi::Framebuffer framebuffer;
	rndrboi::CommandManager command_manager;

	static VulkanAPI* singleton_instance;

    };

}

#endif
