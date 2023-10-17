#include "systems/rendering_system.hpp"
#include "vulkan_wrappers/vulkan_api_helpers.hpp"

#include <set>
#include <algorithm>
#include <limits>

#define ATTENTION_PRINT (A_BLUE "[RENDERING SYSTEM] " A_RESET)
#define OK_PRINT (A_YELLOW "[RENDERING SYSTEM] " A_RESET)
#define BAD_PRINT (A_RED "[RENDERING SYSTEM] " A_RESET)

using namespace rndrboi;


RenderingSystem* RenderingSystem::singleton_instance = NULL;

RenderingSystem* RenderingSystem::Instance()
{
    if (!singleton_instance)
    {
	singleton_instance = new RenderingSystem;
    }
    return singleton_instance;
}


void RenderingSystem::init()
{
    // choose device
    rndrboi::VulkanDevicePreferences dev_preferences{};
    dev_preferences.graphics		= true;
    dev_preferences.present		= true;
    dev_preferences.limit_one_queue	= true;
    dev_preferences.debug		= true;
    dev_preferences.print_info		= true;
    device_data = VulkanDeviceInit::init( dev_preferences );

    // create swapchain
    swapchain.create( device_data );

    // render passes
    render_pass.create( device_data,
			{
			    .format = swapchain.image_format
			});

    // pipelines
    pipeline.create( device_data,
		     render_pass,
		     {
			 .vert_shader_path	= "./compiled_shaders/shader.vert.spv",
			 .frag_shader_path	= "./compiled_shaders/shader.frag.spv",
			 .viewport_width	= (float)swapchain.width(),
			 .viewport_height	= (float)swapchain.height(),
			 .blend_type		= OFF
		     });

    framebuffer.create( device_data, swapchain, render_pass );

    command_manager.create( device_data, {} );

    command_manager.begin_recording();
    command_manager.begin_render_pass( render_pass, swapchain, framebuffer, 0 );
    command_manager.draw( pipeline, swapchain, 3 );
    command_manager.end_render_pass();
    command_manager.end_recording();
}

void RenderingSystem::step()
{
    std::cout << ATTENTION_PRINT
	      << "step\n";
}

void RenderingSystem::cleanup()
{
    command_manager.clean();
    framebuffer.clean();
    render_pass.clean();
    pipeline.clean();
    swapchain.clean();
    VulkanDeviceInit::clean( device_data );

}
