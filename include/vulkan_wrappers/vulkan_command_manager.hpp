#pragma once

#include <vector>
#include "vulkan_wrappers/vulkan_render_pass.hpp"
#include "vulkan_wrappers/vulkan_swapchain.hpp"
#include "vulkan_wrappers/vulkan_types.hpp"
#include "vulkan_wrappers/vulkan_framebuffer.hpp"
#include "vulkan_wrappers/vulkan_graphics_pipeline.hpp"
#include "vulkan_wrappers/vulkan_sync.hpp"
#include <vulkan/vulkan.hpp>

namespace rndrboi
{
    class CommandManager
    {
    public:
	CommandManager();

	void create( VulkanDevice& dev, CommandManagerSettings settings );

	void clean();

	void reset( int buffer_index = 0 );

	void begin_recording( int buffer_index = 0 );

	void end_recording( int buffer_index = 0 );

	void begin_render_pass( RenderPass& render_pass,
				Swapchain& swapchain,
				Framebuffer& framebuffer,
				int image_index,
				int command_buffer_index = 0 );

	void end_render_pass( int command_buffer_index = 0 );

	void draw( GraphicsPipeline& pipeline,
		   Swapchain& swapchain,
		   int vert_count,
		   int buffer_index = 0 );

	void submit( Semaphore& wait_sem, Semaphore signal_sem, Fence completion_fence );

	void present( Swapchain& swapchain, uint32_t image_index, Semaphore& wait_sem );

	VkCommandPool command_pool;
	std::vector <VkCommandBuffer> command_buffers;

    private:
	VulkanDevice* internal_dev;

    };
}
