#pragma once

#include "window.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "scene.hpp"
#include "vulkan_wrappers/vulkan_types.hpp"
#include "vulkan_wrappers/vulkan_device.hpp"
#include "vulkan_wrappers/vulkan_swapchain.hpp"
#include "vulkan_wrappers/vulkan_graphics_pipeline.hpp"
#include "vulkan_wrappers/vulkan_render_pass.hpp"
#include "vulkan_wrappers/vulkan_framebuffer.hpp"
#include "vulkan_wrappers/vulkan_command_manager.hpp"
#include "vulkan_wrappers/vulkan_sync.hpp"
#include "vulkan_wrappers/vulkan_buffer_manager.hpp"
#include "vulkan_wrappers/vulkan_descriptor_manager.hpp"
#include <vulkan/vulkan.h>
#include "vulkan_wrappers/vulkan_texture.hpp"

#include "mesh.hpp"


namespace rndrboi
{

    struct UniformModelViewProjection
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    };

    class RenderingSystem
    {
    public:

        void init();
        void setup( Scene& scene );
        void step( Scene& scene );
        void cleanup();

        static RenderingSystem* Instance();

    private:

        RenderingSystem(){}
        ~RenderingSystem(){ cleanup(); }
        void recreate_swapchain();

        rndrboi::VulkanDevice device_data;
        rndrboi::Swapchain swapchain;
        rndrboi::RenderPass render_pass;
        rndrboi::GraphicsPipeline pipeline;
        rndrboi::Framebuffer framebuffer;
        rndrboi::CommandManager command_manager;

        rndrboi::Semaphore sem_image_available;
        rndrboi::Semaphore sem_render_finished;
        rndrboi::Fence fence_frame_in_flight;

        rndrboi::DescriptorManager uniform_manager;

        UniformModelViewProjection model_view_projection;
        void* model_view_projection_ptr;

        rndrboi::Buffer* vertex_buffer_staging;
        rndrboi::Buffer* vertex_buffer;
        void* vertex_buff_staging_ptr;

        rndrboi::Buffer* index_buffer;

        MaterialTextured* material_template_textured;

        static RenderingSystem* singleton_instance;

    };

};
