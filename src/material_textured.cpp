#include "material_textured.hpp"


using namespace rndrboi;

MaterialTextured::MaterialTextured()
{
    descriptor_manager.create( Device::Instance()->device );
    descriptor_manager.add_dummy_sampler( 0 ); // diffuse
    descriptor_manager.add_dummy_sampler( 1 ); // specular
    descriptor_manager.add_dummy_sampler( 2 ); // bump
    descriptor_manager.done();

    diffuse_texture.create( Device::Instance()->device );
}

void MaterialTextured::set_diffuse( std::string path )
{
    diffuse_texture.load( path );
    descriptor_manager.update_sampler( 0, diffuse_texture.image_view );
    descriptor_manager.done();

    has_diffuse = true;
}

void MaterialTextured::clean()
{
    if( has_diffuse )
        diffuse_texture.clean();

    descriptor_manager.clean();
}
