#pragma once

#include "mesh.hpp"

namespace rndrboi
{
    // ----------------------------------------------------------------------------------------------------

    typedef int32_t ASSET_ID;
    typedef ASSET_ID MESH_ID;

    // ----------------------------------------------------------------------------------------------------

    class AssetManager
    {
    public:

	static AssetManager* Instance();

	MESH_ID add_mesh();
	MESH_ID add_cube();
	Mesh& get_mesh( MESH_ID id );

    private:
	AssetManager();
	~AssetManager(){}

	std::vector<Mesh*> meshes;
	MESH_ID default_mesh;

	static AssetManager* instance;
    };

    // ----------------------------------------------------------------------------------------------------

};
