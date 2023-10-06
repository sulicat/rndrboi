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
}

bool Engine::is_running()
{
    return window->is_open();
}


void Engine::step()
{
    window->handle_input();
}
