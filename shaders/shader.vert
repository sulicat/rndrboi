#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;

layout(location = 0) out vec3 pos_out;
layout(location = 1) out vec3 normal_out;
layout(location = 2) out vec2 uv_out;
layout(location = 3) out vec4 color_out;


layout(set = 0, binding = 0) uniform ModelViewProjection {
    mat4 model;
    mat4 view;
    mat4 projection;
} mvp;

void main() {
    gl_Position = mvp.projection * mvp.view * mvp.model * vec4(pos.xyz, 1.0);

    pos_out = pos;
    normal_out = normal;
    uv_out = uv;
    color_out = color;
}
