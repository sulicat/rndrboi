#include "systems/rendering_system.hpp"
#include "vulkan_wrappers/vulkan_api_helpers.hpp"
#include "vulkan_wrappers/vulkan_vertex.hpp"
#include "components/renderable.hpp"
#include "components/mesh_component.hpp"


#include <set>
#include <algorithm>
#include <limits>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    BufferManager::Instance()->init( device_data );

    // uniforms
    uniform_manager.create( device_data );

    // model view projection
    Uniform* uniform_mvp = uniform_manager.add_uniform<UniformModelViewProjection>("model_view_projection", 0);
    model_view_projection_ptr = uniform_mvp->data_ptr;

    uniform_manager.done();

    // pipelines
    pipeline.create( device_data,
		     render_pass,
		     {
			 .vert_shader_path	= "./compiled_shaders/shader.vert.spv",
			 .frag_shader_path	= "./compiled_shaders/shader.frag.spv",
			 .viewport_width	= (float)swapchain.width(),
			 .viewport_height	= (float)swapchain.height(),
			 .shader_attributes	= { { 0, Vertex::offset_pos(), VK_FORMAT_R32G32B32_SFLOAT },
						    { 1, Vertex::offset_normal(), VK_FORMAT_R32G32B32_SFLOAT },
						    { 2, Vertex::offset_uv(), VK_FORMAT_R32G32_SFLOAT },
						    { 3, Vertex::offset_color(), VK_FORMAT_R32G32B32A32_SFLOAT } },
			 .descriptor_layouts	= { uniform_manager.get_layout() },
		     });

    // create a vertex buffer
    vertex_buffer = BufferManager::Instance()->get_buffer({ .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT });
    vertex_buff_ptr = BufferManager::Instance()->get_mapped_memory( *vertex_buffer );

    // create a vertex buffer
    index_buffer = BufferManager::Instance()->get_buffer({ .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT });
    void* index_buff_ptr = BufferManager::Instance()->get_mapped_memory( *index_buffer );

    // test texture
    wood_texture.create( device_data );
    wood_texture.load( "resources/textures/wood_light.jpg" );


    framebuffer.create( device_data, swapchain, render_pass );

    command_manager.create( device_data, {} );
}

void RenderingSystem::step( Scene& scene )
{
    // wait till the last frame is done drawing
    fence_frame_in_flight.wait();

    // check if a resize has occured
    auto image_res = swapchain.acquire_next_image( sem_image_available );
    uint32_t image_index = image_res.first;

    if( image_res.second == SWAPCHAIN_STATUS::OUT_OF_DATE )
    {
	recreate_swapchain();
	return;
    }

    fence_frame_in_flight.reset();

    // ----------------------------------------------------------------------------------------------------

    // get all renderables
    auto renderables_view = scene.registry->view<components::Renderable, components::Mesh>();

    glm::mat4 model_mat = glm::mat4(1.0f);
    glm::mat4 rotate_mat = glm::mat4(1.0f);
    glm::mat4 scale_mat = glm::mat4(1.0f);
    glm::mat4 translate_mat = glm::mat4(1.0f);

    model_view_projection.view = scene.camera.get_view_mat();
    model_view_projection.projection = scene.camera.get_projection_mat();

    command_manager.reset();
    command_manager.begin_recording();
    command_manager.begin_render_pass( render_pass, swapchain, framebuffer, image_index );

    // every components
    for(auto ent: renderables_view)
    {
        auto& mesh_comp = renderables_view.get<components::Mesh>(ent);
	auto& mesh = AssetManager::Instance()->get_mesh( mesh_comp.mesh_id );

	memcpy( vertex_buff_ptr,
		mesh.vertex_data.data(), mesh.vertex_data.size() * sizeof(Vertex));

	command_manager.bind_vertex_buffer( vertex_buffer );
	command_manager.bind_index_buffer( index_buffer );

	model_view_projection.model = model_mat * translate_mat * rotate_mat * scale_mat;;

	memcpy( model_view_projection_ptr,
		&model_view_projection,
		sizeof(UniformModelViewProjection) );


	command_manager.draw( pipeline,
			      swapchain,
			      { &uniform_manager },
			      mesh.vertex_data.size(),
			      false );
    }

    command_manager.end_render_pass();
    command_manager.end_recording();

    command_manager.submit( sem_image_available, sem_render_finished, fence_frame_in_flight );
    command_manager.present( swapchain, image_index, sem_render_finished );
}

void RenderingSystem::recreate_swapchain()
{
    VulkanDeviceInit::wait_idle( device_data );

    VulkanDeviceInit::update_swapchain_info( device_data );

    framebuffer.clean();
    swapchain.clean();

    swapchain.create( device_data );
    framebuffer.create( device_data, swapchain, render_pass );

    pipeline.update_size( swapchain.width(), swapchain.height() );
}

void RenderingSystem::cleanup()
{
    VulkanDeviceInit::wait_idle( device_data );

    uniform_manager.clean();

    sem_image_available.clean();
    sem_render_finished.clean();
    fence_frame_in_flight.clean();

    command_manager.clean();
    framebuffer.clean();
    render_pass.clean();
    pipeline.clean();
    swapchain.clean();

    BufferManager::Instance()->clean();
    VulkanDeviceInit::clean( device_data );

}
