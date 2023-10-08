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

    VulkanAPI::Instance()->init_default();

}

bool Engine::is_running()
{
    return window->is_open();
}


void Engine::step()
{
    window->handle_input();
}

int Engine::close()
{
    VulkanAPI::Instance()->cleanup();
    Window::Instance()->cleanup();

    return 0;
}
