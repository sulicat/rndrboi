#include "components/mesh_component.hpp"
#include "utils.hpp"

using namespace rndrboi::components;

Mesh::Mesh()
{
    std::cout << A_BG(100,100,0) << A_RESET << "component\n";
}

Mesh::Mesh( MESH_DEFAULT type )
{
    if( type == MESH_DEFAULT::CUBE )
	mesh_id = AssetManager::Instance()->add_cube();
}

Mesh::Mesh( std::string path )
{
    std::cout << A_BG(100,100,0) << "component: " << path << A_RESET << "\n";
}

