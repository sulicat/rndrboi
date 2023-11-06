#pragma once

#include "vulkan_wrappers/vulkan_types.hpp"
#include "vulkan_wrappers/vulkan_buffer_manager.hpp"
#include "vulkan_wrappers/vulkan_texture.hpp"
#include <vulkan/vulkan.hpp>

#include "utils.hpp"

#define OK_PRINT_UM (A_YELLOW "[VULKAN UNIFORM MANAGER] " A_RESET)
#define BAD_PRINT_UM (A_RED "[VULKAN UNIFORM MANAGER] " A_RESET)

namespace rndrboi
{
    struct Uniform
    {
    void* data_ptr;
    int max_size;
    int size;
    int bind_point;
    std::string name;
    rndrboi::Buffer* buffer;
    };

    struct Sampler
    {
    int bind_point;
    VkSampler sampler;
    VkImageView image_view;
    };


    class UniformManager
    {
    public:
    UniformManager();
    void create( VulkanDevice& dev, uint32_t descriptor_set_binding_in = 0 );

    template <typename T>
    Uniform* add_uniform( std::string name, uint32_t bind_point )
    {
        Uniform* out = new Uniform;
        out->size = sizeof(T);
        out->name = name;
        out->bind_point = bind_point;

        // create a buffer to map for this uniform
        out->buffer = BufferManager::Instance()->get_buffer( { .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT } );
        out->data_ptr = BufferManager::Instance()->get_mapped_memory( *out->buffer );

        std::cout << OK_PRINT_UM << "Created uniform: " << name << "\n";

        uniforms.push_back( out );

        return uniforms[uniforms.size()-1];
    }

    Sampler* add_sampler( int bind_point_in, VkImageView image_view_in );
    Sampler* add_dummy_sampler( int bind_point_in );

    VkDescriptorSetLayout new_layout();
    VkDescriptorSetLayout get_layout();
    void done();
    void clean();

    VkDescriptorSetLayout layout;
    VkDescriptorSet descriptor_set;
    VkDescriptorPool descriptor_pool;
    uint32_t descriptor_set_binding;

    private:
    void create_dummy_texture();

    VulkanDevice* internal_device;
    std::vector<Uniform*> uniforms;
    std::vector<Sampler*> samplers;

    bool dummy_texture_created = false;
    rndrboi::VulkanTexture dummy_texture;
    };

};
