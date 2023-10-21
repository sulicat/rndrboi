#include "vulkan_wrappers/vulkan_uniform_manager.hpp"

#define OK_PRINT (A_YELLOW "[VULKAN UNIFORM MANAGER] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN UNIFORM MANAGER] " A_RESET)

using namespace rndrboi;

UniformManager::UniformManager()
{

}

void UniformManager::create( VulkanDevice& dev )
{
    internal_device = &dev;

}

void UniformManager::done()
{
    if( uniforms.size() <= 0 )
    {
	std::cout << OK_PRINT << "No uniforms, not binding any descriptors\n";
	return;
    }

    // create descriptor pool ------------------------------------------------------------

    VkDescriptorPoolSize pool_size{};
    pool_size.type		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_size.descriptorCount	= (uint32_t)uniforms.size();

    VkDescriptorPoolCreateInfo pool_info{};
    pool_info.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.poolSizeCount	= 1;
    pool_info.pPoolSizes	= &pool_size;
    pool_info.maxSets		= (uint32_t)uniforms.size();

    VkResult res = vkCreateDescriptorPool( internal_device->logical_device,
					   &pool_info,
					   nullptr,
					   &descriptor_pool);

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not create descriptor pool\n";

    // create descriptor sets ------------------------------------------------------------
    std::vector<VkDescriptorSetLayout> layouts = get_layouts();

    VkDescriptorSetAllocateInfo alloc_info{};
    alloc_info.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool		= descriptor_pool;
    alloc_info.descriptorSetCount	= (uint32_t)uniforms.size();
    alloc_info.pSetLayouts		= layouts.data();

    descriptor_sets.resize( (uint32_t)uniforms.size() );

    res = vkAllocateDescriptorSets( internal_device->logical_device,
				    &alloc_info,
				    descriptor_sets.data() );

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not create descriptor sets\n";

    // configure descriptors in the sets --------------------------------------------------
    for( int i = 0; i < uniforms.size(); i++ )
    {
	Uniform* uni = uniforms[i];
	std::cout << OK_PRINT << "  " << "configuring: " << uni->name << "\n";

	VkDescriptorBufferInfo buffer_info{};
	buffer_info.buffer = uni->buffer.buffer;
	buffer_info.offset = 0;
	buffer_info.range = uni->size;

	VkWriteDescriptorSet descriptor_write{};
        descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptor_write.dstSet = descriptor_sets[i];
	descriptor_write.dstBinding = uni->bind_point;
	descriptor_write.dstArrayElement = 0;
	descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptor_write.descriptorCount = 1;
	descriptor_write.pBufferInfo = &buffer_info;
	//descriptor_write.pImageInfo = nullptr; // Optional
	//descriptor_write.pTexelBufferView = nullptr; // Optional


	vkUpdateDescriptorSets( internal_device->logical_device,
				1,
				&descriptor_write,
				0,
				nullptr);
    }

}

std::vector<VkDescriptorSetLayout> UniformManager::get_layouts()
{
    std::vector<VkDescriptorSetLayout> out;

    for( auto uni : uniforms )
	out.push_back( uni->descriptor_set.descriptor_set_layout );

    return out;
}

void UniformManager::clean()
{
}
