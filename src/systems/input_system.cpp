#include "systems/input_system.hpp"
#include "utils.hpp"
#include <iostream>

using namespace rndrboi;

InputSystem* InputSystem::p_instance = NULL;

InputSystem::InputSystem(){}
InputSystem::~InputSystem(){}

InputSystem* InputSystem::Instance(){
    if( p_instance == NULL ){
        p_instance = new InputSystem();
    }
    return p_instance;
}

void InputSystem::step()
{
}

void InputSystem::set_window_size( float w, float h )
{
    window_width = w;
    window_height = h;
}

void InputSystem::trigger_mouse_move( double x, double y )
{
    double dx = x - last_mouse_x;
    double dy = y - last_mouse_y;

    last_mouse_x = x;
    last_mouse_y = y;

    double dx_percent = dx / (float)window_width;
    double dy_percent = dy / (float)window_height;

    // check if the mouse is held down, then send that drag event
    if( is_mouse_down[MouseButton::MOUSE_LEFT] == true )
    {
        for( int i = 0; i < mouse_drag_callbacks.size(); i++ ){
            mouse_drag_callbacks[i]( MouseButton::MOUSE_LEFT, x, y, dx, dy, dx_percent, dy_percent );
        }
    }

    if( is_mouse_down[MouseButton::MOUSE_RIGHT] == true )
    {
        for( int i = 0; i < mouse_drag_callbacks.size(); i++ ){
            mouse_drag_callbacks[i]( MouseButton::MOUSE_RIGHT, x, y, dx, dy, dx_percent, dy_percent );
        }
    }

    if( is_mouse_down[MouseButton::MOUSE_MIDDLE] == true )
    {
        for( int i = 0; i < mouse_drag_callbacks.size(); i++ ){
            mouse_drag_callbacks[i]( MouseButton::MOUSE_MIDDLE, x, y, dx, dy, dx_percent, dy_percent );
        }
    }

}

void InputSystem::trigger_mouse_down( enum MouseButton button )
{
    is_mouse_down[button] = true;

    for( int i = 0; i < mouse_click_callbacks.size(); i++ ){
        mouse_down_callbacks[i]( button, last_mouse_x, last_mouse_y );
    }
}

void InputSystem::trigger_mouse_release( enum MouseButton button )
{
    is_mouse_down[button] = false;

    for( int i = 0; i < mouse_release_callbacks.size(); i++ ){
        mouse_release_callbacks[i]( button, last_mouse_x, last_mouse_y );
    }
}

void InputSystem::trigger_mouse_enter()
{
}

void InputSystem::trigger_mouse_leave()
{
}

void InputSystem::trigger_window_resize( int x, int y )
{
    window_width = x;
    window_height = y;

    for( int i = 0; i < window_resize_callbacks.size(); i++ ){
        window_resize_callbacks[i]( x, y );
    }
}

void InputSystem::trigger_scroll( double x, double y )
{
    for( int i = 0; i < mouse_scroll_callbacks.size(); i++ ){
        mouse_scroll_callbacks[i]( last_mouse_x, last_mouse_y, y );
    }
}

// ----------------------------------------------------------------------------------------------------

void InputSystem::register_cb_mouse_down( MouseDownCB cb ){
    mouse_down_callbacks.push_back( cb );
}

void InputSystem::register_cb_mouse_move( MouseMoveCB cb ){
    mouse_move_callbacks.push_back( cb );
}

void InputSystem::register_cb_mouse_drag( MouseDragCB cb ){
    mouse_drag_callbacks.push_back( cb );
}

void InputSystem::register_cb_mouse_release( MouseReleaseCB cb ){
    mouse_release_callbacks.push_back( cb );
}

void InputSystem::register_cb_mouse_double_click( MouseDoubleClickCB cb ){
    mouse_double_click_callbacks.push_back( cb );
}

void InputSystem::register_cb_mouse_scroll( MouseScrollCB cb ){
    mouse_scroll_callbacks.push_back( cb );
}

void InputSystem::register_cb_window_resize( WindowResizeCB cb ){
    window_resize_callbacks.push_back( cb );
}
