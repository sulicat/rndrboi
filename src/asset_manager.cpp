#include "asset_manager.hpp"
#include "utils.hpp"
#include "vertex_data/vert_cube.hpp"

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

AssetManager::AssetManager()
{
    default_mesh = add_cube();
}

Mesh& AssetManager::get_mesh( MESH_ID id )
{
    if( id < 0 || id >= meshes.size() )
	return get_mesh(default_mesh);

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

    int vert_size = 8;
    int vert_count = CUBE_VERT_SIZE / vert_size;
    mesh.vertex_data.resize( vert_count );

    for( int i = 0; i < vert_count; i++ )
    {
	int offs_i = i*vert_size;

	mesh.vertex_data[i].pos[0] = VERT_NORM_UV_data_cube[offs_i + 0];
	mesh.vertex_data[i].pos[1] = VERT_NORM_UV_data_cube[offs_i + 1];
	mesh.vertex_data[i].pos[2] = VERT_NORM_UV_data_cube[offs_i + 2];

	mesh.vertex_data[i].normal[0] = VERT_NORM_UV_data_cube[offs_i + 3];
	mesh.vertex_data[i].normal[1] = VERT_NORM_UV_data_cube[offs_i + 4];
	mesh.vertex_data[i].normal[2] = VERT_NORM_UV_data_cube[offs_i + 5];

	mesh.vertex_data[i].uv[0] = VERT_NORM_UV_data_cube[offs_i + 6];
	mesh.vertex_data[i].uv[1] = VERT_NORM_UV_data_cube[offs_i + 7];
    }

    return mesh_id;
}
