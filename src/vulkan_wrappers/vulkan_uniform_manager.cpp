#include "vulkan_wrappers/vulkan_uniform_manager.hpp"

#define OK_PRINT (A_YELLOW "[VULKAN UNIFORM MANAGER] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN UNIFORM MANAGER] " A_RESET)

using namespace rndrboi;

UniformManager::UniformManager()
{

}

void UniformManager::create( VulkanDevice& dev, uint32_t descriptor_set_binding_in)
{
    internal_device = &dev;

    descriptor_set_binding = descriptor_set_binding_in;
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
    layout = new_layout();

    VkDescriptorSetAllocateInfo alloc_info{};
    alloc_info.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool		= descriptor_pool;
    alloc_info.descriptorSetCount	= 1;
    alloc_info.pSetLayouts		= &layout;

    res = vkAllocateDescriptorSets( internal_device->logical_device,
				    &alloc_info,
				    &descriptor_set );

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not create descriptor sets\n";

    // configure descriptors in the sets --------------------------------------------------
    for( int i = 0; i < uniforms.size(); i++ )
    {
	Uniform* uni = uniforms[i];

	VkDescriptorBufferInfo buffer_info{};
	buffer_info.buffer	= uni->buffer.buffer;
	buffer_info.offset	= 0;
	buffer_info.range	= uni->size;

	VkWriteDescriptorSet descriptor_write{};
        descriptor_write.sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptor_write.dstSet			= descriptor_set;
	descriptor_write.dstBinding		= uni->bind_point;
	descriptor_write.dstArrayElement	= 0;
	descriptor_write.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptor_write.descriptorCount	= 1;
	descriptor_write.pBufferInfo		= &buffer_info;
	//descriptor_write.pImageInfo		= nullptr; // Optional
	//descriptor_write.pTexelBufferView	= nullptr; // Optional

	vkUpdateDescriptorSets( internal_device->logical_device,
				1,
				&descriptor_write,
				0,
				nullptr);
    }

}

VkDescriptorSetLayout UniformManager::new_layout()
{
    VkDescriptorSetLayout out;

    std::vector<VkDescriptorSetLayoutBinding> layout_bindings;
    layout_bindings.resize( uniforms.size() );

    for( int i = 0; i < uniforms.size(); i++ )
    {
	layout_bindings[i].binding		= uniforms[i]->bind_point;
	layout_bindings[i].descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layout_bindings[i].descriptorCount	= 1;
	layout_bindings[i].stageFlags		= VK_SHADER_STAGE_ALL_GRAPHICS;
	layout_bindings[i].pImmutableSamplers	= nullptr;
    }

    VkDescriptorSetLayoutCreateInfo layout_create_info{};
    layout_create_info.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_create_info.bindingCount	= layout_bindings.size();
    layout_create_info.pBindings	= layout_bindings.data();

    VkResult res = vkCreateDescriptorSetLayout( internal_device->logical_device,
						&layout_create_info,
						nullptr,
						&out );

    if( res != VK_SUCCESS )
	std::cout << BAD_PRINT << "ERROR could not create descriptor set layout\n";

    return std::move(out);
}

VkDescriptorSetLayout UniformManager::get_layout()
{
    return layout;
}

void UniformManager::clean()
{
    for( auto& uni : uniforms )
    {
	std::cout << OK_PRINT << " Cleaned uniform: " << uni->name << "\n";

	BufferManager::Instance()->clean_buffer(uni->buffer);
	delete uni;
    }

    vkDestroyDescriptorPool( internal_device->logical_device,
			     descriptor_pool,
			     nullptr );

    vkDestroyDescriptorSetLayout( internal_device->logical_device,
				  layout,
				  nullptr );
}
