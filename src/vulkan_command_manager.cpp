#include "utils.hpp"
#include "vulkan_command_manager.hpp"

#define OK_PRINT (A_YELLOW "[VULKAN COMMANDPOOL] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN COMMANDPOOL] " A_RESET)


using namespace rndrboi;

CommandManager::CommandManager()
{
}

void CommandManager::create( VulkanDevice& dev, CommandManagerSettings settings )
{
    internal_dev = &dev;

    VkCommandPoolCreateInfo pool_create_info{};
    pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    // unsafe, but we are assuming we have atleast 1 graphics family
    pool_create_info.queueFamilyIndex = dev.queue_fam_info.graphics_family_indices[0];

    VkResult res = vkCreateCommandPool( dev.logical_device, &pool_create_info, nullptr, &command_pool );

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not create command pool\n";


    // create command buffers
    if( settings.num_command_buffers < 1 )
    {
	std::cout << OK_PRINT << "requested: " << settings.num_command_buffers << " Command buffers... min is 1, creating min\n";
	settings.num_command_buffers = 1;
    }

    command_buffers.resize( settings.num_command_buffers );

    for( int i = 0; i < settings.num_command_buffers; i++ )
    {
	VkCommandBufferAllocateInfo allocate_info{};
	allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocate_info.commandPool = command_pool;
	allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocate_info.commandBufferCount = 1;

	res = vkAllocateCommandBuffers( dev.logical_device, &allocate_info, &command_buffers[i] );

	if( res != VK_SUCCESS )
	    std::cout << BAD_PRINT << "ERROR Failed to allocate command buffer: " << i << "\n";
    }
}

void CommandManager::begin_recording( int buffer_index )
{
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType		= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags		= 0;
    begin_info.pInheritanceInfo = nullptr;

    VkResult res = vkBeginCommandBuffer(command_buffers[buffer_index], &begin_info);

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not begin recording on buffer: " << buffer_index << "\n";
}


void CommandManager::end_recording( int buffer_index )
{

    VkResult res = vkEndCommandBuffer( command_buffers[buffer_index] );
    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR: Failed to record command buffer\n";
}

void CommandManager::end_render_pass( int buffer_index )
{
    vkCmdEndRenderPass( command_buffers[buffer_index] );
}

void CommandManager::begin_render_pass( RenderPass& render_pass,
					Swapchain& swapchain,
					Framebuffer& framebuffer,
					int image_index,
					int command_buffer_index )
{

    VkRenderPassBeginInfo render_pass_info{};
    render_pass_info.sType		= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass		= render_pass.render_pass;
    render_pass_info.framebuffer	= framebuffer.swapchain_framebuffers[image_index];
    render_pass_info.renderArea.offset	= {0, 0};
    render_pass_info.renderArea.extent	= swapchain.image_extent;

    VkClearValue clear_color = {{{ 1.0f, 0.0f, 1.0f, 1.0f }}};
    render_pass_info.clearValueCount = 1;
    render_pass_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(command_buffers[0], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

}


void CommandManager::draw( GraphicsPipeline& pipeline,
			   Swapchain& swapchain,
			   int vert_count,
			   int buffer_index )
{
    vkCmdBindPipeline( command_buffers[ buffer_index ],
		       VK_PIPELINE_BIND_POINT_GRAPHICS,
		       pipeline.pipeline );

    vkCmdSetViewport	( command_buffers[ buffer_index ], 0, 1, &pipeline.viewport );
    vkCmdSetScissor	( command_buffers[ buffer_index ], 0, 1, &pipeline.scissor );
    vkCmdDraw		( command_buffers[ buffer_index ], vert_count, 1, 0, 0 );
}


void CommandManager::clean()
{
    vkDestroyCommandPool( internal_dev->logical_device, command_pool, nullptr );
}
