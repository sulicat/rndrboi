#include "camera.hpp"

#include <iostream>
#include "glm/gtx/string_cast.hpp"

using namespace rndrboi;

Camera::Camera()
{
    pos = glm::vec3(5.0, 0.0, -1.0);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 0.0f, 1.0f);

    width = 1920;
    height = 1080;

    orbit_sensitivity = 500;
    zoom_sensitivity = 3;
}

Camera::~Camera()
{
}

glm::vec3 Camera::get_camera_pos()
{
    return pos;
}

glm::mat4 Camera::get_view_mat()
{
    return glm::lookAt( pos,
                        target,
                        up );
}

glm::mat4 Camera::get_projection_mat()
{
    projection_mat = glm::perspective( glm::radians(45.0f),
                                       (float)width/(float)height,
                                       0.1f, 100.0f );
    projection_mat[1][1] *= -1;

    return projection_mat;
}

glm::vec3 Camera::get_camera_rotation()
{
    glm::vec3 camera_direction = glm::normalize(target - pos);
    float up_z_angle = glm::acos( glm::dot(up, glm::vec3(0.0f, 0.0f, 1.0f) ) );

    float direction_y_angle = glm::acos( glm::dot(camera_direction, glm::vec3(0.0f, 1.0f, 0.0f) ) );


    return glm::vec3( up_z_angle, direction_y_angle, 0.0f );
}

void Camera::set_size( int w, int h )
{
    width = w;
    height = h;
}

void Camera::orbit( float dx, float dy )
{
    orbit_angle( dx * orbit_sensitivity,
                 dy * orbit_sensitivity  );
}

void Camera::orbit_angle( float angle_x, float angle_y )
{
    glm::mat4 rotate_mat_x = glm::mat4(1.0f);
    glm::mat4 rotate_mat_y = glm::mat4(1.0f);
    glm::vec3 camera_direction = glm::normalize(pos - target);

    glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, 1.0f);

    // turntable type rotation on x axis
    rotate_mat_x = glm::rotate( rotate_mat_x,
                                glm::radians(angle_x),
                                z_axis );

    // rotate along camera right
    rotate_mat_y = glm::rotate( rotate_mat_y,
                                glm::radians(-angle_y),
                                glm::normalize(glm::cross(camera_direction, up)) );

    // calculate the position with the applied orbit rotation
    glm::vec4 pos4 = glm::vec4(pos, 0);
    pos4 = pos4 * rotate_mat_y * rotate_mat_x;
    pos = glm::vec3(pos4);

    // recalcute the up vector
    up = glm::normalize(glm::vec3( glm::vec4(up, 0.0f) * rotate_mat_y * rotate_mat_x ));
}

void Camera::zoom( float dist )
{
    if( dist * zoom_sensitivity < glm::distance(pos, target) ){
        zoom_dist( dist * zoom_sensitivity );
    }
}

void Camera::zoom_dist( float dist )
{
    glm::vec3 camera_direction = glm::normalize(target - pos);
    pos += camera_direction * dist;
}

void Camera::pan( float x, float y )
{
    glm::mat4 view_mat = get_view_mat();
    glm::vec3 camera_direction = glm::normalize(target - pos);
    glm::vec3 camera_right = glm::cross( camera_direction, up );

    pos += up * y;
    target += up * y;

    pos -= camera_right * x;
    target -= camera_right * x;
}
