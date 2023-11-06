#include "utils.hpp"
#include "window.hpp"
#include "systems/rendering_system.hpp"
#include "systems/input_system.hpp"
#include "scene.hpp"

namespace rndrboi
{
    class Engine
    {
    public:
    Engine();
    ~Engine();
    bool is_running();
    void init();
    void step();
    int close();

    Scene create_scene();
    void set_scene( Scene& new_scene );
    Scene& get_scene();

    private:
    std::shared_ptr<WindowBase> window;
    Scene* scene;
    };

};
