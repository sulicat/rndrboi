#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "rndrboi.hpp"

int main( int argc, char** argv )
{
    std::cout << "STARTING SIMPLE TEST\n";

    rndrboi::Engine engine;
    engine.init();


    while( engine.is_running() )
    {
	engine.step();
    }

    return 0;
}
