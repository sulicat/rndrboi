#include "window.hpp"

using namespace rndrboi;

// ----------------------------------------------------------------------------------------------------

Window* Window::instance = NULL;

Window* Window::Instance(){
    if (!instance){
	instance = new Window;
	instance->platform_window = std::shared_ptr<WindowBase>(new WindowGLFW);
    }
    return instance;
}

std::shared_ptr<WindowBase> Window::get()
{
    std::cout << A_YELLOW << "[WINDOW] " << A_RESET << "GLFW WINDOW\n";
    return platform_window;
}

void Window::cleanup()
{
    std::cout << A_YELLOW << "[WINDOW] " << A_RESET << "cleanup\n";
    platform_window->cleanup();
}

// ----------------------------------------------------------------------------------------------------

WindowGLFW::WindowGLFW()
{
    width = 500;
    height = 500;
    title = "rndrboi";
}

WindowGLFW::~WindowGLFW()
{
    free(window);
}

void WindowGLFW::cleanup()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowGLFW::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width,
			      height,
			      title.c_str(),
			      nullptr,
			      nullptr);

}

void WindowGLFW::handle_input()
{
    glfwPollEvents();
}

bool WindowGLFW::is_open()
{
    return !glfwWindowShouldClose(window);
}
