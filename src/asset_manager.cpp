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
    default_material = add_material();
}

Mesh* AssetManager::get_mesh( MESH_ID id )
{
    if( id < 0 || id >= meshes.size() )
        return NULL;

    return meshes[id];
}

MODEL_ID AssetManager::add_model()
{
    Model* new_model = new Model;
    models.push_back(new_model);
    return models.size()-1;
}

Model* AssetManager::get_model( MODEL_ID id )
{
    if( id < 0 || id >= models.size() )
        return NULL;

    return models[id];
}


void AssetManager::add_mesh_to_model( MODEL_ID model_id, MESH_ID mesh_id )
{
    Model* model = get_model( model_id );
    if( model != NULL )
        model->meshes.push_back( std::make_pair( mesh_id, default_material ) );
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
    Mesh* mesh = get_mesh( mesh_id );

    int vert_size = 8;
    int vert_count = CUBE_VERT_SIZE / vert_size;
    mesh->vertex_data.resize( vert_count );

    for( int i = 0; i < vert_count; i++ )
    {
        int offs_i = i*vert_size;

        mesh->vertex_data[i].pos[0] = VERT_NORM_UV_data_cube[offs_i + 0];
        mesh->vertex_data[i].pos[1] = VERT_NORM_UV_data_cube[offs_i + 1];
        mesh->vertex_data[i].pos[2] = VERT_NORM_UV_data_cube[offs_i + 2];

        mesh->vertex_data[i].normal[0] = VERT_NORM_UV_data_cube[offs_i + 3];
        mesh->vertex_data[i].normal[1] = VERT_NORM_UV_data_cube[offs_i + 4];
        mesh->vertex_data[i].normal[2] = VERT_NORM_UV_data_cube[offs_i + 5];

        mesh->vertex_data[i].uv[0] = VERT_NORM_UV_data_cube[offs_i + 6];
        mesh->vertex_data[i].uv[1] = VERT_NORM_UV_data_cube[offs_i + 7];
    }

    return mesh_id;
}


MATERIAL_ID AssetManager::add_material()
{
    MaterialTextured* new_mat = new MaterialTextured;
    materials.push_back( new_mat );
    return materials.size()-1;
}

MaterialTextured* AssetManager::get_material( MATERIAL_ID id )
{
    if( id < 0 || id >= materials.size() )
        return NULL;

    return materials[id];
}

void AssetManager::cleanup()
{
    for( int i = 0; i < meshes.size(); i++ )
    {
        //meshes[i]->clean();
        delete meshes[i];
    }

    for( int i = 0; i < materials.size(); i++ )
    {
        materials[i]->clean();
        delete materials[i];
    }


    for( int i = 0; i < models.size(); i++ )
    {
        //models[i]->clean();
        delete models[i];
    }

}

// ----------------------------------------------------------------------------------------------------

Mesh* Model::mesh(int i)
{
    return AssetManager::Instance()->get_mesh( meshes[i].first );
}

MaterialTextured* Model::material(int i)
{
    return AssetManager::Instance()->get_material( meshes[i].second );
}

int Model::size()
{
    return meshes.size();
}


