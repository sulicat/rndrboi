#version 450


layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} mvp;

layout(binding = 1) uniform MVP {
    float col;
} mvp2;

layout(location = 3) out vec3 fragColor;

void main() {
    gl_Position = mvp.projection * mvp.view * mvp.model * vec4(pos.xyz, 1.0);
    //gl_Position = vec4(pos.xyz, 1.0);
    fragColor = vec3(color.xy, mvp2.col);
}
