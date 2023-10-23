#include "scene.hpp"
#include "systems/input_system.hpp"
#include "utils.hpp"

#define ATTENTION_PRINT (A_BLUE "[SCENE] " A_RESET)
#define OK_PRINT (A_YELLOW "[SCENE] " A_RESET)
#define BAD_PRINT (A_RED "[SCENE] " A_RESET)

using namespace rndrboi;

Scene::Scene()
{
    registry = new entt::basic_registry<uint32_t>;
    hook_inputs();
}

Scene::~Scene()
{
    delete registry;
}

void Scene::hook_inputs()
{
    InputSystem::Instance()->register_cb_window_resize( [this]( int w, int h ){
	std::cout << OK_PRINT << "resize: " << w << " " << h << "\n";
	camera.set_size( w, h );
    });

    InputSystem::Instance()->register_cb_mouse_scroll( [this]( double x, double y, float scroll ){
	camera.zoom( scroll );
    });

    InputSystem::Instance()->register_cb_mouse_drag( [this]( InputSystem::MouseButton button,
							     double x, double y,
							     double dx, double dy,
							     double dx_percent, double dy_percent )
    {
	if( button == InputSystem::MouseButton::MOUSE_RIGHT )
	    camera.pan( dx_percent * 10, dy_percent * 10 );
	else if( button == InputSystem::MouseButton::MOUSE_LEFT )
	    camera.orbit( dx_percent, dy_percent );
    });
}

uint32_t Scene::create_entity()
{
    uint32_t ent_out = registry->create();
    return ent_out;
}
