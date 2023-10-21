#pragma once

#include "vulkan_wrappers/vulkan_types.hpp"
#include "vulkan_wrappers/vulkan_buffer_manager.hpp"
#include "vulkan_wrappers/vulkan_descriptor_set.hpp"
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
	DescriptorSet descriptor_set;
	rndrboi::Buffer buffer;
    };

    class UniformManager
    {
    public:
	UniformManager();
	void create( VulkanDevice& dev );

	template <typename T>
	Uniform* add_uniform( std::string name, uint32_t bind_point )
	{
	    Uniform* out = new Uniform;
	    out->size = sizeof(T);
	    out->name = name;
	    out->bind_point = bind_point;
	    out->descriptor_set.create( *internal_device,
				       { .binding = bind_point } );

	    // create a buffer to map for this uniform
	    out->buffer = BufferManager::Instance()->get_buffer( { .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT } );
	    out->data_ptr = BufferManager::Instance()->get_mapped_memory( out->buffer );

	    std::cout << OK_PRINT_UM << "Created uniform: " << name << "\n";

	    uniforms.push_back( out );

	    return uniforms[uniforms.size()-1];
	}

	std::vector<VkDescriptorSetLayout> get_layouts();
	void done();
	void clean();

	std::vector<VkDescriptorSet> descriptor_sets;
	VkDescriptorPool descriptor_pool;

    private:
	VulkanDevice* internal_device;
	std::vector<Uniform*> uniforms;
    };

};
