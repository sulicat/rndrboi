#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "utils.hpp"

namespace rndrboi
{
    class Config
    {
    public:
    static Config* Instance();

    enum WINDOW_MANAGER
    {
        GLFW,
        SDL,
        NONE
    };

    const bool enable_debug = true;
    const enum WINDOW_MANAGER window_manager = WINDOW_MANAGER::GLFW;

    private:
    Config();
    ~Config();

    static Config* instance;

    };

}

#endif
