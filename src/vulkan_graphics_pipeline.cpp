#include "vulkan_graphics_pipeline.hpp"
#include "utils.hpp"

#define OK_PRINT (A_YELLOW "[VULKAN GRAPHICS PIPELINE] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN GRAPHICS PIPELINE] " A_RESET)

using namespace rndrboi;

GraphicsPipeline::GraphicsPipeline()
{

}

void GraphicsPipeline::create( VulkanDevice& dev, GraphicsPipelineSettings settings_in )
{
    settings = settings_in;
    dev_internal = &dev;

    vert_shader = rndrboi::utils::read_file( settings.vert_shader_path );
    frag_shader = rndrboi::utils::read_file( settings.frag_shader_path );

    vert_module = create_shader_module( dev, vert_shader );
    frag_module = create_shader_module( dev, frag_shader );


    VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
    vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_stage_info.module = vert_module;
    vert_shader_stage_info.pName = settings.vert_entrypoint.c_str();

    VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
    frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_info.module = frag_module;
    frag_shader_stage_info.pName = settings.frag_entrypoint.c_str();

    VkPipelineShaderStageCreateInfo shader_stages[] = {
	vert_shader_stage_info,
	frag_shader_stage_info
    };

}

void GraphicsPipeline::clean()
{
    vkDestroyShaderModule( dev_internal->logical_device, vert_module, nullptr );
    vkDestroyShaderModule( dev_internal->logical_device, frag_module, nullptr );
}

VkShaderModule GraphicsPipeline::create_shader_module( VulkanDevice& dev, std::vector<char>& code )
{
    VkShaderModule module;

    VkShaderModuleCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = code.size();
    create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkResult res = vkCreateShaderModule(dev.logical_device, &create_info, nullptr, &module);

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "Could not create shader module\n";

    return std::move(module);
}
