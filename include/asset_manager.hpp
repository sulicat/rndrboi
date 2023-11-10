#pragma once

#include "mesh.hpp"
#include "texture.hpp"
#include "material_textured.hpp"

namespace rndrboi
{
    // ----------------------------------------------------------------------------------------------------

    typedef int32_t ASSET_ID;
    typedef ASSET_ID MESH_ID;
    typedef ASSET_ID MATERIAL_ID;
    typedef ASSET_ID MODEL_ID;

    // ----------------------------------------------------------------------------------------------------

    struct Model
    {
        std::vector<MESH_ID> meshes;
        std::vector<MATERIAL_ID> materials;
    };

    // ----------------------------------------------------------------------------------------------------

    class AssetManager
    {
    public:

        static AssetManager* Instance();

        MESH_ID add_mesh();
        MESH_ID add_cube();
        Mesh& get_mesh( MESH_ID id );

        MATERIAL_ID add_textured_material();
        MaterialTextured& get_material( MATERIAL_ID id );

    private:
        AssetManager();
        ~AssetManager(){}

        std::vector<Mesh*> meshes;
        std::vector<MaterialTextured*> materials;
        std::vector<Model*> models;


        MESH_ID default_mesh;

        static AssetManager* instance;
    };

    // ----------------------------------------------------------------------------------------------------

};
