#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace rndrboi
{

    class Camera
    {
    public:
	glm::vec3 pos;
	glm::vec3 target;
	glm::vec3 up;
	glm::mat4 projection_mat;
	int width;
	int height;
	float orbit_sensitivity;
	float zoom_sensitivity;

	Camera();
	~Camera();
	void set_size( int w, int h );
	glm::vec3 get_camera_pos();
	glm::mat4 get_view_mat();
	glm::mat4 get_projection_mat();
	glm::vec3 get_camera_rotation();

	// orbit based on a sensitivity
	void orbit( float dx, float dy );
	// orbit by given angle in degrees
	void orbit_angle( float angle_x, float angle_y );
	// zoom using the sensitivity and constraints
	void zoom( float dist );
	// zoom by some distance, can be used to bypass constraints
	void zoom_dist( float dist );
	// pan by some distance
	void pan( float x, float y );
    };

};


