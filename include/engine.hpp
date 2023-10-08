#include "utils.hpp"
#include "window.hpp"
#include "vulkan_api.hpp"

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

    private:
	std::shared_ptr<WindowBase> window;
    };

};
