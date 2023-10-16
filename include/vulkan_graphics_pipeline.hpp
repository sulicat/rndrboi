#pragma once

#include <vector>
#include "vulkan_render_pass.hpp"
#include "vulkan_types.hpp"
#include <vulkan/vulkan.hpp>

namespace rndrboi
{
    class GraphicsPipeline
    {
    public:
	GraphicsPipeline();
	void create( VulkanDevice& dev, RenderPass& render_pass, GraphicsPipelineSettings settings );
	VkShaderModule create_shader_module( VulkanDevice& dev, std::vector<char>& code );
	void clean();

	std::vector<char>	 vert_shader;
	std::vector<char>	 frag_shader;
	VkShaderModule		 vert_module;
	VkShaderModule		 frag_module;
	VkViewport		 viewport;
	VkRect2D		 scissor;
	VkPipelineLayout	 pipeline_layout;

    private:
	VulkanDevice* dev_internal;
	GraphicsPipelineSettings settings;

	const std::vector<VkDynamicState> dynamic_states = {
	    VK_DYNAMIC_STATE_VIEWPORT,
	    VK_DYNAMIC_STATE_SCISSOR
	};
    };

};
