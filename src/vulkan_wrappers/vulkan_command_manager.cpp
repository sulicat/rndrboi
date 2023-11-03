#include "utils.hpp"
#include "vulkan_wrappers/vulkan_command_manager.hpp"

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
    pool_create_info.sType		= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_create_info.flags		= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_create_info.queueFamilyIndex	= dev.queue_fam_info.graphics_family_index;

    VkResult res = vkCreateCommandPool( dev.logical_device, &pool_create_info, nullptr, &command_pool );

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not create command pool\n";


    // create command buffers
    if( settings.num_command_buffers < 1 )
    {
	std::cout << OK_PRINT << "requested: " << settings.num_command_buffers << " Command buffers... min is 1, creating min\n";
	settings.num_command_buffers = 1;
    }

    VkCommandBufferAllocateInfo allocate_info{};
    allocate_info.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocate_info.commandPool		= command_pool;
    allocate_info.level			= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocate_info.commandBufferCount	= 1;

    res = vkAllocateCommandBuffers( dev.logical_device, &allocate_info, &command_buffer );

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR Failed to allocate command buffer\n";
}

void CommandManager::reset()
{
    vkResetCommandBuffer( command_buffer, 0 );
}


void CommandManager::begin_recording()
{
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType		= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags		= 0;
    begin_info.pInheritanceInfo = nullptr;

    VkResult res = vkBeginCommandBuffer( command_buffer, &begin_info );

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not begin recording on buffer\n";
}


void CommandManager::end_recording()
{

    VkResult res = vkEndCommandBuffer( command_buffer );
    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR: Failed to record command buffer\n";
}

void CommandManager::end_render_pass()
{
    vkCmdEndRenderPass( command_buffer );
}

void CommandManager::begin_render_pass( RenderPass& render_pass,
					Swapchain& swapchain,
					Framebuffer& framebuffer,
					int image_index )
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

    vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

}

void CommandManager::bind_vertex_buffer( Buffer* buff )
{
    VkBuffer buffers[] = {buff->buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(command_buffer, 0, 1, buffers, offsets);
}

void CommandManager::bind_index_buffer( Buffer* buff )
{
    vkCmdBindIndexBuffer(command_buffer, buff->buffer, 0, VK_INDEX_TYPE_UINT16);
}

void CommandManager::bind_descriptor_sets( GraphicsPipeline& pipeline,
					   std::vector<UniformManager*> uniform_managers)

{
    std::vector<VkDescriptorSet> sets = std::vector<VkDescriptorSet>(uniform_managers.size());
    for( int i = 0; i < uniform_managers.size(); i++ )
	sets[i] = uniform_managers[i]->descriptor_set;

    vkCmdBindDescriptorSets( command_buffer,
			     VK_PIPELINE_BIND_POINT_GRAPHICS,
			     pipeline.pipeline_layout,
			     0,
			     sets.size(),
			     sets.data(),
			     0,
			     nullptr );
}

void CommandManager::draw( GraphicsPipeline& pipeline,
			   Swapchain& swapchain,
			   std::vector<UniformManager*> uniform_managers,
			   int vert_count,
			   bool is_indexed )
{
    vkCmdBindPipeline( command_buffer,
		       VK_PIPELINE_BIND_POINT_GRAPHICS,
		       pipeline.pipeline );

    vkCmdSetViewport	( command_buffer, 0, 1, &pipeline.viewport );
    vkCmdSetScissor	( command_buffer, 0, 1, &pipeline.scissor );

    bind_descriptor_sets( pipeline, uniform_managers );

    if( is_indexed )
	vkCmdDrawIndexed ( command_buffer, vert_count, 1, 0, 0, 0 );
    else
	vkCmdDraw ( command_buffer, vert_count, 1, 0, 0 );
}

void CommandManager::submit( Semaphore& wait_sem, Semaphore signal_sem, Fence completion_fence )
{

    VkSemaphore wait_semaphores[] = { wait_sem.vk_sem };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT };
    VkSemaphore signal_semaphores[] = { signal_sem.vk_sem };

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    VkResult res = vkQueueSubmit( internal_dev->graphics_queue, 1, &submit_info, completion_fence.vk_fence );

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not submit command buffer\n";
}

void CommandManager::present( Swapchain& swapchain, uint32_t image_index, Semaphore& wait_sem )
{
    VkSwapchainKHR swapchains[] = {swapchain.swapchain};
    VkSemaphore wait_semaphores[] = {wait_sem.vk_sem};

    VkPresentInfoKHR present_info{};
    present_info.sType			= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount	= 1;
    present_info.pWaitSemaphores	= wait_semaphores;
    present_info.swapchainCount		= 1;
    present_info.pSwapchains		= swapchains;
    present_info.pImageIndices		= &image_index;
    present_info.pResults		= nullptr;

    vkQueuePresentKHR(internal_dev->present_queue, &present_info);
}


void CommandManager::clean()
{
    vkDestroyCommandPool( internal_dev->logical_device, command_pool, nullptr );
}
