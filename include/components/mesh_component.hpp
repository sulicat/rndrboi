#pragma once

#include <string>
#include "asset_manager.hpp"


namespace rndrboi::components
{

    class Mesh
    {
    public:

    enum MESH_DEFAULT
    {
        CUBE,
        SPHERE,
    };

    Mesh();
    Mesh( MESH_DEFAULT type );
    Mesh( std::string path );

    rndrboi::MESH_ID mesh_id;
    };

};
