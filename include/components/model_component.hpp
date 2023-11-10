#pragma once


#include <string>
#include "asset_manager.hpp"


namespace rndrboi::components
{

    class Model
    {
    public:
        Mesh();

        std::vector<rndrboi::MESH_ID> meshes;
    };

};
