#include "scene.hpp"

using namespace rndrboi;

Scene::Scene()
{

    registry = new entt::basic_registry<uint32_t>;
}

Scene::~Scene()
{
    delete registry;
}

uint32_t Scene::create_entity()
{
    uint32_t ent_out = registry->create();
    return ent_out;
}
