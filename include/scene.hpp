#pragma once

#include "asset_manager.hpp"
#include "camera.hpp"
#include "entt/entt.hpp"

namespace rndrboi
{
    class Engine;

    //----------------------------------------------------------------------------------------------------

    class Scene
    {
    public:
    ~Scene();
    uint32_t create_entity();

    template <typename T, typename... TA>
    T& add_component( uint32_t entity, TA&&... args )
    {
        return registry->emplace<T>( entity, std::forward<TA>(args)... );
    }

    template <typename T>
    T& get_component( uint32_t entity )
    {
        return registry->get<T>(entity);
    }

    template <typename T>
    uint32_t remove_component( uint32_t entity )
    {
        return registry->remove<T>(entity);
    }

    entt::basic_registry<uint32_t>* registry;
    Camera camera;

    private:
    Scene();
    void hook_inputs();

    friend class Engine;
    };

    //----------------------------------------------------------------------------------------------------

};
