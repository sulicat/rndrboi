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
    window->handle_input();
    RenderingSystem::Instance()->step();
}

int Engine::close()
{
    RenderingSystem::Instance()->cleanup();
    Window::Instance()->cleanup();
    return 0;
}
