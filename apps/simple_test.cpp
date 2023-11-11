#include <iostream>

#include "rndrboi.hpp"


int main( int argc, char** argv )
{
    std::cout << "STARTING SIMPLE TEST\n";

    rndrboi::Engine engine;
    engine.init();

    rndrboi::Scene& scene = engine.get_scene();
    uint32_t cube = scene.create_entity();

    scene.add_component<rndrboi::components::Renderable>              (cube);
    scene.add_component<rndrboi::components::Transform>               (cube);

    auto model_comp = scene.add_component<rndrboi::components::Model> (cube, rndrboi::components::MODEL_DEFAULT::CUBE);
    rndrboi::Model* model = model_comp.get_model();
    // cube loads in withb 1 mesh
    model->material(0)->set_diffuse("resources/textures/wood_light.jpg");


    while( engine.is_running() )
    {
	engine.step();
    }

    int close_status = engine.close();

    return close_status;
}
