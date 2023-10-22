#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;

layout(set = 0, binding = 0) uniform ModelViewProjection {
    mat4 model;
    mat4 view;
    mat4 projection;
} mvp;

layout(location = 3) out vec3 fragColor;

void main() {
    gl_Position = mvp.projection * mvp.view * mvp.model * vec4(pos.xyz, 1.0);
    fragColor = vec3(color.xyz);
}
