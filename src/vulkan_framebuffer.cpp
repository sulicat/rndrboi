#include "utils.hpp"
#include "vulkan_framebuffer.hpp"


#define OK_PRINT (A_YELLOW "[VULKAN FRAMEBUFFER] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN FRAMEBUFFER] " A_RESET)

using namespace rndrboi;

Framebuffer::Framebuffer()
{
}


void Framebuffer::create( VulkanDevice& dev, Swapchain& swapchain, RenderPass& render_pass )
{
    internal_dev = &dev;
    internal_swapchain = &swapchain;
    internal_render_pass = &render_pass;


    swapchain_framebuffers.resize( swapchain.size() );

    for( int i = 0; i < swapchain.image_views.size(); i++ )
    {
	VkImageView attachments[] = { swapchain.image_views[i] };

	VkFramebufferCreateInfo framebuffer_create_info{};
	framebuffer_create_info.sType		= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebuffer_create_info.renderPass	= render_pass.render_pass;
	framebuffer_create_info.attachmentCount = 1;
	framebuffer_create_info.pAttachments	= attachments;
	framebuffer_create_info.width		= swapchain.width();
	framebuffer_create_info.height		= swapchain.height();
	framebuffer_create_info.layers		= 1;

	VkResult res = vkCreateFramebuffer( dev.logical_device, &framebuffer_create_info, nullptr, &swapchain_framebuffers[i] );

	if( res != VK_SUCCESS )
	    std::cout << BAD_PRINT << "ERROR: could not create framebuffer\n";

    }
}

void Framebuffer::clean()
{
    for( auto f : swapchain_framebuffers )
	vkDestroyFramebuffer( internal_dev->logical_device, f, nullptr );
}
