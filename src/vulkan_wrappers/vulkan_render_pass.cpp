#include "vulkan_wrappers/vulkan_render_pass.hpp"
#include "utils.hpp"
#include <iostream>

#define OK_PRINT (A_YELLOW "[VULKAN RENDERPASS] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN RENDERPASS] " A_RESET)

using namespace rndrboi;

RenderPass::RenderPass()
{

}

void RenderPass::create( VulkanDevice& dev, RenderPassSettings settings )
{
    dev_internal = &dev;

    VkAttachmentDescription color_attachment{};
    color_attachment.format		= settings.format;
    color_attachment.samples		= VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp		= VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp		= VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_LOAD;
    color_attachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout	= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref{};
    color_attachment_ref.attachment	= 0;
    color_attachment_ref.layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount	= 1;
    subpass.pColorAttachments		= &color_attachment_ref;


    VkRenderPassCreateInfo render_pass_create_info{};
    render_pass_create_info.sType		= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.attachmentCount	= 1;
    render_pass_create_info.pAttachments	= &color_attachment;
    render_pass_create_info.subpassCount	= 1;
    render_pass_create_info.pSubpasses		= &subpass;

    VkResult res = vkCreateRenderPass( dev.logical_device, &render_pass_create_info, nullptr, &render_pass );
    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR Could not create renderpass\n";

}

void RenderPass::clean()
{
    vkDestroyRenderPass( dev_internal->logical_device, render_pass, nullptr );
}
