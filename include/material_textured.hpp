#pragma once

#include "material_base.hpp"


namespace rndrboi
{

    class MaterialTextured : public MaterialBase
    {
    public:
        MaterialTextured();
        void set_diffuse( std::string path );

        VulkanTexture diffuse_texture;
    };

};
