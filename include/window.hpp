#include "utils.hpp"

namespace rndrboi
{
    // ----------------------------------------------------------------------------------------------------

    class WindowBase
    {
    public:
	virtual ~WindowBase(){};
	virtual void init() = 0;
	virtual void handle_input() = 0;
	virtual bool is_open() = 0;

	int width;
	int height;
	std::string title;
    };

    // ----------------------------------------------------------------------------------------------------

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

    class WindowGLFW : public WindowBase
    {
    public:

	WindowGLFW();
	~WindowGLFW();
	void init();
	void handle_input();
	bool is_open();

	GLFWwindow* window;
    };


    // ----------------------------------------------------------------------------------------------------
    // singleton instance, returns the correct window based on desired platform
    class Window
    {
    public:

	static Window* Instance();
	std::shared_ptr<WindowBase> get();

    private:
	Window(){}
	~Window(){}

	static Window* instance;
    };


}
