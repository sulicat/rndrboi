#pragma once

#include "asset_manager.hpp"

namespace rndrboi
{
    class Engine;

    //----------------------------------------------------------------------------------------------------

    class Scene
    {
    public:
	~Scene();

	int id;

    private:
	Scene();

	friend class Engine;
    };

    //----------------------------------------------------------------------------------------------------

};
