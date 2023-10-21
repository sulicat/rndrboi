#include "systems/rendering_system.hpp"
#include "vulkan_wrappers/vulkan_api_helpers.hpp"

#include "vulkan_wrappers/vulkan_vertex.hpp"

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

const std::vector<Vertex> triangle_verts = {
    { {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
    { { 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} },
    { {-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f} }
};

const std::vector<uint16_t> indices = {
    0,1,2,2,3,0
};

struct ModelViewProjection
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

struct TestUniform
{
    float col;
};

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

    uniform_manager.create( device_data );
    Uniform* uniform_mvp = uniform_manager.add_uniform<ModelViewProjection>("mvp", 0);
    mvp_buff_ptr = uniform_mvp->data_ptr;

    Uniform* uniform_test_float = uniform_manager.add_uniform<TestUniform>("test", 1);
    test_float_buff_ptr = uniform_test_float->data_ptr;

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
						    { 1, Vertex::offset_color(), VK_FORMAT_R32G32B32A32_SFLOAT } },
			 .descriptor_layouts	= { uniform_manager.get_layout() },
			 });

    // create a vertex buffer
    vertex_buffer = BufferManager::Instance()->get_buffer( { .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT } );
    void* vertex_buff_ptr = BufferManager::Instance()->get_mapped_memory( vertex_buffer );
    memcpy( vertex_buff_ptr, triangle_verts.data(), triangle_verts.size() * sizeof(Vertex));

    // create a vertex buffer
    index_buffer = BufferManager::Instance()->get_buffer({ .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT });
    void* index_buff_ptr = BufferManager::Instance()->get_mapped_memory( index_buffer );
    memcpy( index_buff_ptr, indices.data(), indices.size() * sizeof(uint16_t));


    framebuffer.create( device_data, swapchain, render_pass );

    command_manager.create( device_data, {} );
}

void RenderingSystem::step()
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

                                                                                
    static float i = 0;
    i += 0.01;

    ModelViewProjection mvp;
    mvp.model =  glm::rotate( glm::mat4(1.0f), i* glm::radians(90.0f),
			      glm::vec3(0.0f, 0.0f, 1.0f) );

    mvp.view =  glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
			    glm::vec3(0.0f, 0.0f, 0.0f),
			    glm::vec3(0.0f, 0.0f, 1.0f));

    mvp.projection = glm::perspective(glm::radians(45.0f),
				      (float)swapchain.width() / (float)swapchain.height(), 0.1f,
				      10.0f);
    memcpy( mvp_buff_ptr,
	    &mvp,
	    sizeof(ModelViewProjection) );

    float col_temp = sin(i * 0.5);
    memcpy( test_float_buff_ptr,
	    &col_temp,
	    sizeof(float) );

                                                                                

    command_manager.reset();

    command_manager.begin_recording();
    command_manager.begin_render_pass( render_pass, swapchain, framebuffer, image_index );

    command_manager.bind_vertex_buffer( vertex_buffer );
    command_manager.bind_index_buffer( index_buffer );

    command_manager.draw( pipeline, swapchain, uniform_manager, indices.size(), true );
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

    BufferManager::Instance()->clean_buffer(index_buffer);
    BufferManager::Instance()->clean_buffer(vertex_buffer);
    BufferManager::Instance()->clean();

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
