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

	void reset();

	void begin_recording();

	void end_recording();

	void begin_render_pass( RenderPass& render_pass,
				Swapchain& swapchain,
				Framebuffer& framebuffer,
				int image_index );

	void end_render_pass();

	void bind_vertex_buffer( VkBuffer buff );

	void draw( GraphicsPipeline& pipeline,
		   Swapchain& swapchain,
		   int vert_count );

	void submit( Semaphore& wait_sem, Semaphore signal_sem, Fence completion_fence );

	void present( Swapchain& swapchain, uint32_t image_index, Semaphore& wait_sem );

	VkCommandPool command_pool;
	VkCommandBuffer command_buffer;

    private:
	VulkanDevice* internal_dev;

    };
}
