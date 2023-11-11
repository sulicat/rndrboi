#include "components/model_component.hpp"
#include "utils.hpp"

using namespace rndrboi::components;

Model::Model()
{
    create_empty();
}

Model::Model( MODEL_DEFAULT type_in )
{
    // start with an empty model
    create_empty();

    if( type_in == MODEL_DEFAULT::CUBE || 1 == 1 )
    {
        // create an cube mesh
        MESH_ID mesh_id = AssetManager::Instance()->add_cube();
        AssetManager::Instance()->add_mesh_to_model( model_id, mesh_id );
    }

}

void Model::create_empty()
{
    model_id = AssetManager::Instance()->add_model();
}


rndrboi::Model* rndrboi::components::Model::get_model()
{
    return rndrboi::AssetManager::Instance()->get_model( model_id );
}
