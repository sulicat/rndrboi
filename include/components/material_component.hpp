#pragma once

#include <string>
#include "asset_manager.hpp"
#include "material_textured.hpp"

namespace rndrboi::components
{
    class Material
    {
    public:
        Material();
        void set_diffuse( std::string path );

        rndrboi::MATERIAL_ID mat;
    };

};
