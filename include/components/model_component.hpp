#pragma once


#include <string>
#include "asset_manager.hpp"


namespace rndrboi::components
{

    enum MODEL_DEFAULT
    {
        CUBE,
        SPHERE,
    };


    class Model
    {
    public:
        Model();
        Model( MODEL_DEFAULT type_in );
        void create_empty();

        MODEL_ID model_id;
    };

};
