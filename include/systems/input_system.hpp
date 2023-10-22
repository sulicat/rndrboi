#pragma once

#include <stddef.h>
#include <vector>
#include <functional>

namespace rndrboi{

    class InputSystem
    {
    public:

	enum MouseButton{
	    MOUSE_LEFT	 = 0b001,
	    MOUSE_RIGHT	 = 0b010,
	    MOUSE_MIDDLE = 0b100
	};

	// how long before the drag is considered a drag?
	const float mouse_drag_threshold = 1.0f;

	// args:  x, y, dx, dy
	typedef std::function<void(int, int, int, int)> MouseMoveCB;
	// args: btn, x, y
	typedef std::function<void(enum MouseButton, double, double)> MouseReleaseCB;
	// args: btn, x, y
	typedef std::function<void(enum MouseButton, double, double)> MouseClickCB;
	// args: btn, x, y
	typedef std::function<void(enum MouseButton, double, double)> MouseDoubleClickCB;
	// ags: btn, x, y, duration
	typedef std::function<void(enum MouseButton, double, double)> MouseDownCB;
	// ags: btn, x, y, drag_x, drag_y
	typedef std::function<void(enum MouseButton, double, double, double, double, double, double)> MouseDragCB;
	// args: x, y, scroll_amount
	typedef std::function<void(double, double, float)> MouseScrollCB;
	// args: x, y
	typedef std::function<void(int, int)> WindowResizeCB;

	InputSystem();
	~InputSystem();
	static InputSystem* Instance();

	void step();
	void register_cb_mouse_click( MouseClickCB cb );
	void register_cb_mouse_double_click( MouseDoubleClickCB cb );
	void register_cb_mouse_release( MouseReleaseCB cb );
	void register_cb_mouse_down( MouseDownCB cb );
	void register_cb_mouse_move( MouseMoveCB cb );
	void register_cb_mouse_drag( MouseDragCB cb );
	void register_cb_mouse_scroll( MouseScrollCB cb );
	void register_cb_window_resize( WindowResizeCB cb );

	void trigger_mouse_down( enum MouseButton );
	void trigger_mouse_release( enum MouseButton );
	void trigger_mouse_move( double x, double y );
	void trigger_mouse_enter();
	void trigger_mouse_leave();
	void trigger_window_resize( int x, int y );
	void trigger_scroll( double x, double y );

	void set_window_size( float w, float h );

    private:
	double last_mouse_x;
	double last_mouse_y;
	bool is_mouse_down[16];
	float window_width;
	float window_height;

	static InputSystem* p_instance;
	std::vector<MouseClickCB> mouse_click_callbacks;
	std::vector<MouseDoubleClickCB> mouse_double_click_callbacks;
	std::vector<MouseDownCB> mouse_down_callbacks;
	std::vector<MouseMoveCB> mouse_move_callbacks;
	std::vector<MouseDragCB> mouse_drag_callbacks;
	std::vector<MouseReleaseCB> mouse_release_callbacks;
	std::vector<MouseScrollCB> mouse_scroll_callbacks;
	std::vector<WindowResizeCB> window_resize_callbacks;
    };

};
