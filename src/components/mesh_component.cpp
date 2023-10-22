#include "components/mesh_component.hpp"
#include "utils.hpp"

using namespace rndrboi::components;

Mesh::Mesh()
{
}

Mesh::Mesh( MESH_DEFAULT type )
{
    mesh_id = AssetManager::Instance()->add_cube();
}

Mesh::Mesh( std::string path )
{
}

