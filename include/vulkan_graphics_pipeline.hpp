#pragma once


#include <vector>
#include "vulkan_types.hpp"
#include <vulkan/vulkan.hpp>

namespace rndrboi
{
    class GraphicsPipeline
    {
    public:
	GraphicsPipeline();
	void create( VulkanDevice& dev, GraphicsPipelineSettings settings );
	void clean();

	VkShaderModule create_shader_module( VulkanDevice& dev, std::vector<char>& code );

	std::vector<char> vert_shader;
	std::vector<char> frag_shader;
	VkShaderModule vert_module;
	VkShaderModule frag_module;
    private:
	VulkanDevice* dev_internal;
	GraphicsPipelineSettings settings;
    };

};
