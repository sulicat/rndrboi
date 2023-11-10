#include <iostream>

#include "rndrboi.hpp"


int main( int argc, char** argv )
{
    std::cout << "STARTING SIMPLE TEST\n";

    rndrboi::Engine engine;
    engine.init();

    rndrboi::Scene& scene = engine.get_scene();
    uint32_t cube = scene.create_entity();
    scene.add_component<rndrboi::components::Renderable>                (cube);
    //scene.add_component<rndrboi::components::Model>                     (cube);
    scene.add_component<rndrboi::components::Transform>                 (cube);

    /*
    scene.add_component<rndrboi::components::Material>                  (cube);
    auto& cube_mat = scene.get_component<rndrboi::components::Material> (cube);
    cube_mat.set_diffuse( "resources/textures/wood_light.jpg" );
    */
    /*
    uint32_t susanne = scene.create_entity();
    scene.add_component<rndrboi::components::Renderable>	(susanne);
    scene.add_component<rndrboi::components::Transform>		(susanne);
    */
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
