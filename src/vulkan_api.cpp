#include "vulkan_api.hpp"
#include "vulkan_api_helpers.hpp"

#include <set>
#include <algorithm>
#include <limits>

#define OK_PRINT (A_YELLOW "[VULKAN API] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN API] " A_RESET)

using namespace rndrboi;

VulkanAPI* VulkanAPI::singleton_instance = NULL;

VulkanAPI* VulkanAPI::Instance()
{
    if (!singleton_instance)
    {
	singleton_instance = new VulkanAPI;
    }
    return singleton_instance;
}

//----------------------------------------------------------------------------------------------------

void VulkanAPI::init_default()
{
    rndrboi::VulkanDevicePreferences dev_preferences{};
    dev_preferences.graphics		= true;
    dev_preferences.present		= true;
    dev_preferences.limit_one_queue	= true;
    dev_preferences.debug		= true;
    dev_preferences.print_info		= true;

    device_data = VulkanDeviceInit::init( dev_preferences );

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



}

void VulkanAPI::cleanup()
{
    render_pass.clean();
    pipeline.clean();
    swapchain.clean();
    VulkanDeviceInit::clean( device_data );
}
