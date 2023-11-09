#include "material_textured.hpp"


using namespace rndrboi;

MaterialTextured::MaterialTextured()
{

}

void MaterialTextured::set_diffuse( std::string path )
{
    diffuse_texture.load( path );
}
