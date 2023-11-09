#version 450
layout(location = 0) out vec4 outColor;


layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;

layout(set = 1, binding = 0) uniform sampler2D tex_0;
layout(set = 1, binding = 1) uniform sampler2D tex_1;
layout(set = 1, binding = 2) uniform sampler2D tex_2;
layout(set = 1, binding = 3) uniform sampler2D tex_3;
layout(set = 1, binding = 4) uniform sampler2D tex_4;

void main() {
    outColor = vec4(pos, 1.0);
    //outColor = texture( tex_0, uv );
}
