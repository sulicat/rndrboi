#include "engine.hpp"

using namespace rndrboi;

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::init()
{
    window = Window::Instance()->get();
    window->width = 1080;
    window->height = 720;
    window->title = "rndrboi";
    window->init();

    scene = new Scene();

    Device::Instance()->init(); // initialize graphics card
    BufferManager::Instance()->init(Device::Instance()->device);
    RenderingSystem::Instance()->init();
}

Scene Engine::create_scene()
{
    Scene scene_out;
    return std::move(scene_out);
}

void Engine::set_scene( Scene& new_scene )
{
    scene = &new_scene;
}

Scene& Engine::get_scene()
{
    return *scene;
}

bool Engine::is_running()
{
    return window->is_open();
}

void Engine::step()
{
    // ahh my eyes
    static bool do_once = false;
    if( do_once == false )
    {
        RenderingSystem::Instance()->setup( *scene ); // dirty fix pls
        do_once = true;
    }


    window->handle_input();
    InputSystem::Instance()->step();
    RenderingSystem::Instance()->step( *scene );
}

int Engine::close()
{
    RenderingSystem::Instance()->cleanup();
    AssetManager::Instance()->cleanup();
    BufferManager::Instance()->clean();
    VulkanDeviceInit::clean( Device::Instance()->device );

    Window::Instance()->cleanup();

    return 0;
}
