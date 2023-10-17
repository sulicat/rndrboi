#pragma once

#include "window.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "vulkan_wrappers/vulkan_types.hpp"
#include "vulkan_wrappers/vulkan_init.hpp"
#include "vulkan_wrappers/vulkan_swapchain.hpp"
#include "vulkan_wrappers/vulkan_graphics_pipeline.hpp"
#include "vulkan_wrappers/vulkan_render_pass.hpp"
#include "vulkan_wrappers/vulkan_framebuffer.hpp"
#include "vulkan_wrappers/vulkan_command_manager.hpp"
#include <vulkan/vulkan.h>


namespace rndrboi
{

    class RenderingSystem
    {
    public:

	void init();
	void step();
	void cleanup();

	static RenderingSystem* Instance();

    private:

	RenderingSystem(){}
	~RenderingSystem(){ cleanup(); }

	rndrboi::VulkanDevice device_data;
	rndrboi::Swapchain swapchain;
	rndrboi::RenderPass render_pass;
	rndrboi::GraphicsPipeline pipeline;
	rndrboi::Framebuffer framebuffer;
	rndrboi::CommandManager command_manager;

	static RenderingSystem* singleton_instance;

    };

};
