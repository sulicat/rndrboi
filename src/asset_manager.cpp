#include "asset_manager.hpp"
#include "utils.hpp"

#define ATTENTION_PRINT (A_BLUE "[ASSET MANAGER] " A_RESET)
#define OK_PRINT (A_YELLOW "[ASSET MANAGER] " A_RESET)
#define BAD_PRINT (A_RED "[ASSET MANAGER] " A_RESET)

using namespace rndrboi;

AssetManager* AssetManager::instance = NULL;

AssetManager* AssetManager::Instance()
{
    if (!instance){
	std::cout << OK_PRINT << "Asset Manager Created\n";
	instance = new AssetManager;
    }
    return instance;
}

// ----------------------------------------------------------------------------------------------------

Mesh& AssetManager::get_mesh( MESH_ID id )
{
    return *meshes[id];
}

ASSET_ID AssetManager::add_mesh()
{
    std::cout << ATTENTION_PRINT << "Adding Mesh\n";

    Mesh* new_mesh = new Mesh();
    meshes.push_back( new_mesh );
    return meshes.size()-1;
}

MESH_ID AssetManager::add_cube()
{
    MESH_ID mesh_id = add_mesh();
    Mesh& mesh = get_mesh( mesh_id );

    // fill in vertex info based on baked in cube

    return mesh_id;
}
