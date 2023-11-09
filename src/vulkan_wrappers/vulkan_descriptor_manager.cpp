#include "vulkan_wrappers/vulkan_descriptor_manager.hpp"

#define OK_PRINT (A_YELLOW "[VULKAN DESCRIPTOR MANAGER] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN DESCRIPTOR MANAGER] " A_RESET)

using namespace rndrboi;

DescriptorManager::DescriptorManager()
{

}

void DescriptorManager::create( VulkanDevice& dev, uint32_t descriptor_set_binding_in)
{
    internal_device = &dev;

    descriptor_set_binding = descriptor_set_binding_in;
}

void DescriptorManager::create_dummy_texture()
{
    dummy_texture.create( *internal_device );
    char* temp_data = (char*)malloc( sizeof(char)*(1*1*4) );
    dummy_texture.from_data( temp_data, 1,1,4 );
    dummy_texture_created = true;
    free(temp_data);
}


Sampler* DescriptorManager::add_dummy_sampler( int bind_point_in )
{
    if( !dummy_texture_created )
        create_dummy_texture();

    Sampler* sampler = add_sampler( bind_point_in, dummy_texture.image_view );
    return sampler;
}

Sampler* DescriptorManager::add_sampler( int bind_point_in, VkImageView image_view_in )
{
    Sampler* sampler_out = new Sampler;
    sampler_out->bind_point = bind_point_in;
    sampler_out->image_view = image_view_in;

    VkSamplerCreateInfo sampler_info{};
    sampler_info.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.magFilter               = VK_FILTER_LINEAR;
    sampler_info.minFilter               = VK_FILTER_LINEAR;
    sampler_info.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.anisotropyEnable        = VK_TRUE;
    sampler_info.maxAnisotropy           = internal_device->max_anisotropy;
    sampler_info.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.unnormalizedCoordinates = VK_FALSE;
    sampler_info.compareEnable           = VK_FALSE;
    sampler_info.compareOp               = VK_COMPARE_OP_ALWAYS;
    sampler_info.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_info.mipLodBias              = 0.0f;
    sampler_info.minLod                  = 0.0f;
    sampler_info.maxLod                  = 0.0f;

    VkResult res = vkCreateSampler( internal_device->logical_device,
                                    &sampler_info,
                                    nullptr,
                                    &sampler_out->sampler );

    if( res != VK_SUCCESS )
    {
        std::cout << BAD_PRINT_UM << "Could not create sampler\n";
        return NULL;
    }

    std::cout << OK_PRINT_UM << "Created sampler: " << bind_point_in << "\n";
    samplers.push_back( sampler_out );
    return samplers[ samplers.size()-1];
}



void DescriptorManager::done()
{
    if( uniforms.size() <= 0 && samplers.size() <= 0 )
    {
        std::cout << OK_PRINT << "No uniforms, not binding any descriptors\n";
        return;
    }

    // create descriptor pool ------------------------------------------------------------

    VkDescriptorPoolSize pool_size_uniform{};
    pool_size_uniform.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_size_uniform.descriptorCount = (uint32_t)uniforms.size();

    VkDescriptorPoolSize pool_size_sampler{};
    pool_size_sampler.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    pool_size_sampler.descriptorCount = (uint32_t)samplers.size();

    std::vector<VkDescriptorPoolSize> pool_sizes;

    if( uniforms.size() > 0 )
        pool_sizes.push_back( pool_size_uniform );

    if( samplers.size() > 0 )
        pool_sizes.push_back( pool_size_sampler );

    VkDescriptorPoolCreateInfo pool_info{};
    pool_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.poolSizeCount = pool_sizes.size();
    pool_info.pPoolSizes    = pool_sizes.data();
    pool_info.maxSets       = ( (uint32_t)uniforms.size() + (uint32_t)samplers.size() );

    VkResult res = vkCreateDescriptorPool( internal_device->logical_device,
                                           &pool_info,
                                           nullptr,
                                           &descriptor_pool);

    if( res != VK_SUCCESS )
        std::cout << BAD_PRINT << "ERROR could not create descriptor pool\n";

    // create descriptor sets ------------------------------------------------------------
    layout = new_layout();

    VkDescriptorSetAllocateInfo alloc_info{};
    alloc_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool     = descriptor_pool;
    alloc_info.descriptorSetCount = 1;
    alloc_info.pSetLayouts        = &layout;

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
        buffer_info.buffer  = uni->buffer->buffer;
        buffer_info.offset  = 0;
        buffer_info.range   = uni->size;

        VkWriteDescriptorSet descriptor_write{};
        descriptor_write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write.dstSet          = descriptor_set;
        descriptor_write.dstBinding      = uni->bind_point;
        descriptor_write.dstArrayElement = 0;
        descriptor_write.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_write.descriptorCount = 1;
        descriptor_write.pBufferInfo     = &buffer_info;

        vkUpdateDescriptorSets( internal_device->logical_device,
                                1,
                                &descriptor_write,
                                0,
                                nullptr);
    }

    // configure descriptors in the sets --------------------------------------------------
    for( int i = 0; i < samplers.size(); i++ )
    {
        Sampler* sampler = samplers[i];

        VkDescriptorImageInfo image_info{};
        image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        image_info.imageView = sampler->image_view;
        image_info.sampler = sampler->sampler;

        VkWriteDescriptorSet descriptor_write{};
        descriptor_write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write.dstSet          = descriptor_set;
        descriptor_write.dstBinding      = sampler->bind_point;
        descriptor_write.dstArrayElement = 0;
        descriptor_write.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptor_write.descriptorCount = 1;
        descriptor_write.pImageInfo      = &image_info;

        vkUpdateDescriptorSets( internal_device->logical_device,
                                1,
                                &descriptor_write,
                                0,
                                nullptr);
    }


}

VkDescriptorSetLayout DescriptorManager::new_layout()
{
    VkDescriptorSetLayout out;

    std::vector<VkDescriptorSetLayoutBinding> layout_bindings;
    layout_bindings.resize( uniforms.size() + samplers.size() );

    // uniforms
    for( int i = 0; i < uniforms.size(); i++ )
    {
        layout_bindings[i].binding            = uniforms[i]->bind_point;
        layout_bindings[i].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layout_bindings[i].descriptorCount    = 1;
        layout_bindings[i].stageFlags         = VK_SHADER_STAGE_ALL_GRAPHICS;
        layout_bindings[i].pImmutableSamplers = nullptr;
    }

    // samplers
    int offs = uniforms.size();
    for( int i = 0; i < samplers.size(); i++ )
    {
        layout_bindings[i + offs].binding            = samplers[i]->bind_point;
        layout_bindings[i + offs].descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        layout_bindings[i + offs].descriptorCount    = 1;
        layout_bindings[i + offs].stageFlags         = VK_SHADER_STAGE_ALL_GRAPHICS;
        layout_bindings[i + offs].pImmutableSamplers = nullptr;
    }


    VkDescriptorSetLayoutCreateInfo layout_create_info{};
    layout_create_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_create_info.bindingCount = layout_bindings.size();
    layout_create_info.pBindings    = layout_bindings.data();

    VkResult res = vkCreateDescriptorSetLayout( internal_device->logical_device,
                                                &layout_create_info,
                                                nullptr,
                                                &out );

    if( res != VK_SUCCESS )
        std::cout << BAD_PRINT << "ERROR could not create descriptor set layout\n";

    return std::move(out);
}

VkDescriptorSetLayout DescriptorManager::get_layout()
{
    return layout;
}

void DescriptorManager::clean()
{
    for( auto& sampler : samplers )
    {
        std::cout << OK_PRINT << " Cleaned sampler: " << sampler->bind_point << "\n";
        vkDestroySampler( internal_device->logical_device,
                          sampler->sampler,
                          nullptr );
    }


    for( auto& uni : uniforms )
    {
        std::cout << OK_PRINT << " Cleaned uniform: " << uni->name << "\n";
        delete uni;
    }

    vkDestroyDescriptorPool( internal_device->logical_device,
                             descriptor_pool,
                             nullptr );

    vkDestroyDescriptorSetLayout( internal_device->logical_device,
                                  layout,
                                  nullptr );


    if( dummy_texture_created )
        dummy_texture.clean();
}
