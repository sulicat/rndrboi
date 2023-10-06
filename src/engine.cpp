#include "utils.hpp"
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

}

bool Engine::is_running()
{
    return true;
}


void Engine::step()
{
    std::cout << A_BLINK << A_FG(0,255,0) << "STEP" << A_RESET << " asdasd \n";;
}
