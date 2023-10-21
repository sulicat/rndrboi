#include "utils.hpp"
#include "vulkan_wrappers/vulkan_descriptor_set.hpp"

#define OK_PRINT (A_YELLOW "[VULKAN DESCRIPTOR SET] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN DESCRIPTOR SET] " A_RESET)

using namespace rndrboi;

DescriptorSet::DescriptorSet()
{

}

void DescriptorSet::create( VulkanDevice& dev, DescriptorSetSettings settings )
{
    internal_device = &dev;

    VkDescriptorSetLayoutBinding layout_binding{};
    layout_binding.binding = settings.binding;
    layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layout_binding.descriptorCount = 1; // in case we want an array of uniforms
    layout_binding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
    layout_binding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layout_create_info{};
    layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_create_info.bindingCount = 1;
    layout_create_info.pBindings = &layout_binding;

    VkResult res = vkCreateDescriptorSetLayout( dev.logical_device,
						&layout_create_info,
						nullptr,
						&descriptor_set_layout );

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not create descriptor set layout\n";


}
