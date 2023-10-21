#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "rndrboi.hpp"

int main( int argc, char** argv )
{
    std::cout << "STARTING SIMPLE TEST\n";

    rndrboi::Engine engine;
    engine.init();

    rndrboi::Scene& scene = engine.get_scene();

    //rndrboi::Scene scene2 = engine.create_scene();
    // engine.set_scene( scene2 );

    /*
      Desired:
       - add entities to different scenes
       - Store memory intensive data in the asset manager
       - ability to copy entities from 1 scene to another
       - swap scenes at runtime
       - attach Graphic Pipelines with a different set of shaders to different scenes
       - different scenes can have different camera controls

     */


    while( engine.is_running() )
    {
	engine.step();
    }

    int close_status = engine.close();

    return close_status;
}
