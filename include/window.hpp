#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "utils.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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
	virtual void cleanup() = 0;

	int width;
	int height;
	std::string title;
    };

    // ----------------------------------------------------------------------------------------------------

    class WindowGLFW : public WindowBase
    {
    public:

	WindowGLFW();
	~WindowGLFW();
	void init();
	void handle_input();
	bool is_open();
	void cleanup();

	GLFWwindow* window;
    };


    // ----------------------------------------------------------------------------------------------------
    // singleton instance, returns the correct window based on desired platform
    class Window
    {
    public:

	static Window* Instance();
	std::shared_ptr<WindowBase> get();
	void cleanup();

    private:
	Window(){}
	~Window(){}

	std::shared_ptr<WindowBase> platform_window;
	static Window* instance;
    };


}

#endif
