#include "systems/rendering_system.hpp"
#include "vulkan_wrappers/vulkan_api_helpers.hpp"
#include "vulkan_wrappers/vulkan_vertex.hpp"
#include "components/all.hpp"


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

    device_data = Device::Instance()->device;

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

    material_template_textured = new MaterialTextured;

    // uniforms
    uniform_manager.create( device_data );
    Uniform* uniform_mvp = uniform_manager.add_uniform<UniformModelViewProjection>("model_view_projection", 0);
    model_view_projection_ptr = uniform_mvp->data_ptr;
    uniform_manager.done();

    // pipelines
    pipeline.create( device_data,
                     render_pass,
                     {
                         .vert_shader_path  = "./compiled_shaders/shader.vert.spv",
                         .frag_shader_path  = "./compiled_shaders/shader.frag.spv",

                         .viewport_width    = (float)swapchain.width(),
                         .viewport_height   = (float)swapchain.height(),

                         .shader_attributes = { { 0, Vertex::offset_pos(), VK_FORMAT_R32G32B32_SFLOAT },
                                                { 1, Vertex::offset_normal(), VK_FORMAT_R32G32B32_SFLOAT },
                                                { 2, Vertex::offset_uv(), VK_FORMAT_R32G32_SFLOAT },
                                                { 3, Vertex::offset_color(), VK_FORMAT_R32G32B32A32_SFLOAT } },

                         .descriptor_layouts    = {
                             uniform_manager.get_layout(),
                             material_template_textured->descriptor_manager.get_layout()
                         },

                     });

    // create a vertex buffer
    vertex_buffer = BufferManager::Instance()->get_buffer({ .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT });
    vertex_buff_ptr = BufferManager::Instance()->get_mapped_memory( *vertex_buffer );

    // create a vertex buffer
    index_buffer = BufferManager::Instance()->get_buffer({ .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT });
    void* index_buff_ptr = BufferManager::Instance()->get_mapped_memory( *index_buffer );

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
    auto renderables_view = scene.registry->view< components::Renderable,
                                                  components::Model >();

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
        auto& model_comp = renderables_view.get<components::Model>(ent);
        Model* model = AssetManager::Instance()->get_model( model_comp.model_id );

        for( int i = 0; i < model->size(); i++ )
        {

            Mesh* mesh = model->mesh(i);
            MaterialTextured* material = model->material(i);

            // TODO: suli -> slow here, fix next
            memcpy( vertex_buff_ptr,
                    mesh->vertex_data.data(), mesh->vertex_data.size() * sizeof(Vertex));

            command_manager.bind_vertex_buffer( vertex_buffer );
            command_manager.bind_index_buffer( index_buffer );

            model_view_projection.model = model_mat * translate_mat * rotate_mat * scale_mat;;

            memcpy( model_view_projection_ptr,
                    &model_view_projection,
                    sizeof(UniformModelViewProjection) );

            command_manager.draw( pipeline,
                                  swapchain,
                                  {
                                      &uniform_manager,
                                      &material->descriptor_manager
                                  },
                                  mesh->vertex_data.size(),
                                  false );
        }
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

    material_template_textured->clean();
    delete material_template_textured;

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
