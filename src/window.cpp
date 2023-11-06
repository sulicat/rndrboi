#include "window.hpp"
#include "systems/input_system.hpp"

using namespace rndrboi;

// ----------------------------------------------------------------------------------------------------

Window* Window::instance = NULL;

Window* Window::Instance(){
    if (!instance){
        std::cout << A_YELLOW << "[WINDOW] " << A_RESET << "GLFW WINDOW\n";
        instance = new Window;
        instance->platform_window = std::shared_ptr<WindowBase>(new WindowGLFW);
    }
    return instance;
}

std::shared_ptr<WindowBase> Window::get()
{
    return platform_window;
}

void Window::cleanup()
{
    std::cout << A_YELLOW << "[WINDOW] " << A_RESET << "cleanup\n";
    platform_window->cleanup();
}

// ----------------------------------------------------------------------------------------------------

void mouse_move_cb(GLFWwindow* window, double xpos, double ypos)
{
    InputSystem::Instance()->trigger_mouse_move( xpos, ypos );
}

void mouse_enter_cb(GLFWwindow* window, int entered)
{
    if( entered )
        InputSystem::Instance()->trigger_mouse_enter();
    else
        InputSystem::Instance()->trigger_mouse_leave();
}

void mouse_button_cb(GLFWwindow* window, int button, int action, int mods)
{
    rndrboi::InputSystem::MouseButton button_internal;

    if( button == GLFW_MOUSE_BUTTON_RIGHT )
        button_internal = rndrboi::InputSystem::MOUSE_RIGHT;

    if( button == GLFW_MOUSE_BUTTON_LEFT )
        button_internal = rndrboi::InputSystem::MOUSE_LEFT;

    if( button == GLFW_MOUSE_BUTTON_MIDDLE )
        button_internal = rndrboi::InputSystem::MOUSE_MIDDLE;

    if( action == GLFW_PRESS )
        InputSystem::Instance()->trigger_mouse_down( button_internal );
    else
        InputSystem::Instance()->trigger_mouse_release( button_internal );
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    InputSystem::Instance()->trigger_scroll( xoffset, yoffset );
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    InputSystem::Instance()->trigger_window_resize( width, height );
}

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
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width,
                              height,
                              title.c_str(),
                              nullptr,
                              nullptr);


    glfwSetCursorPosCallback    (window, mouse_move_cb);
    glfwSetCursorEnterCallback  (window, mouse_enter_cb);
    glfwSetMouseButtonCallback  (window, mouse_button_cb);
    glfwSetWindowSizeCallback   (window, window_size_callback);
    glfwSetScrollCallback   (window, scroll_callback);

    InputSystem::Instance()->set_window_size(width, height);

}

void WindowGLFW::handle_input()
{
    glfwPollEvents();
}

bool WindowGLFW::is_open()
{
    return !glfwWindowShouldClose(window);
}
