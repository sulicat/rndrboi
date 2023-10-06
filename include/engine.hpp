#include "utils.hpp"
#include "window.hpp"

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

	std::shared_ptr<WindowBase> window;
    };

};
