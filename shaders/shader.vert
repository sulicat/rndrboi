#version 450


layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

layout(location = 3) out vec3 fragColor;

void main() {
    gl_Position = vec4(pos.xyz, 1.0);
    fragColor = color.xyz;
}
