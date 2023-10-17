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
    std::cout << OK_PRINT << "init\n";

    // choose device
    rndrboi::VulkanDevicePreferences dev_preferences{};
    dev_preferences.graphics		= true;
    dev_preferences.present		= true;
    dev_preferences.limit_one_queue	= true;
    dev_preferences.debug		= true;
    dev_preferences.print_info		= true;
    device_data = VulkanDeviceInit::init( dev_preferences );

    sem_image_available.create( device_data );
    sem_render_finished.create( device_data );
    fence_frame_in_flight.create( device_data, true );

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
}

void RenderingSystem::step()
{
    std::cout << ATTENTION_PRINT << "STEP\n";

    // wait till the last frame is done drawing
    fence_frame_in_flight.wait();
    fence_frame_in_flight.reset();

    uint32_t image_index = swapchain.acquire_next_image( sem_image_available );
    std::cout << ATTENTION_PRINT << "image index: " << image_index << "\n";

    command_manager.reset();

    command_manager.begin_recording();
    command_manager.begin_render_pass( render_pass, swapchain, framebuffer, image_index );
    command_manager.draw( pipeline, swapchain, 3 );
    command_manager.end_render_pass();
    command_manager.end_recording();

    command_manager.submit( sem_image_available, sem_render_finished, fence_frame_in_flight );

    command_manager.present( swapchain, image_index, sem_render_finished );
}

void RenderingSystem::cleanup()
{
    VulkanDeviceInit::wait_idle( device_data );

    sem_image_available.clean();
    sem_render_finished.clean();
    fence_frame_in_flight.clean();

    command_manager.clean();
    framebuffer.clean();
    render_pass.clean();
    pipeline.clean();
    swapchain.clean();
    VulkanDeviceInit::clean( device_data );

}
