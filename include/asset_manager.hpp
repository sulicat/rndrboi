#pragma once

#include "mesh.hpp"
#include "texture.hpp"
#include "material_textured.hpp"
#include "vulkan_wrappers/vulkan_command_manager.hpp"

namespace rndrboi
{
    class AssetManager;

    // ----------------------------------------------------------------------------------------------------

    typedef int32_t ASSET_ID;
    typedef ASSET_ID MESH_ID;
    typedef ASSET_ID MATERIAL_ID;
    typedef ASSET_ID MODEL_ID;

    // ----------------------------------------------------------------------------------------------------

    // every mesh has a material
    typedef std::pair<MESH_ID, MATERIAL_ID> MeshMatPair;

    class Model
    {
    public:

        Mesh* mesh(int i);
        MaterialTextured* material(int i);
        int size();

    private:
        Model(){};
        std::vector<MeshMatPair> meshes;
        friend class AssetManager;

    };

    // ----------------------------------------------------------------------------------------------------

    class AssetManager
    {
    public:

        static AssetManager* Instance();

        // model --------------------------------------------------------------------------------

        // creates an empty model with no meshes or materials
        MODEL_ID add_model();

        // return the model associated with ID. Otherwise NULL
        Model* get_model( MODEL_ID ID );

        // add a mesh to the model. use the default material
        void add_mesh_to_model( MODEL_ID model_id, MESH_ID mesh_id );

        // mesh --------------------------------------------------------------------------------

        // add an empty mesh
        MESH_ID add_mesh();

        // add a cube mesh
        MESH_ID add_cube();

        // get a mesh given a mesh ID. otherwise NULL
        Mesh* get_mesh( MESH_ID id );

        // material -----------------------------------------------------------------------------

        // add a material and return it's ID
        MATERIAL_ID add_material();

        // get the material given a material ID. null on fail
        MaterialTextured* get_material( MATERIAL_ID id );

        void cleanup();


    private:
        AssetManager();
        ~AssetManager(){}

        std::vector<Mesh*>              meshes;
        std::vector<MaterialTextured*>  materials;
        std::vector<Model*>             models;

        MESH_ID default_mesh;
        MATERIAL_ID default_material;

        static AssetManager* instance;
    };

    // ----------------------------------------------------------------------------------------------------

};
