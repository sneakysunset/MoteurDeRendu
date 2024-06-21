#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
uniform float time;
uniform mat4 view_matrix;
out vec3 pos;
out vec2 uv;
out vec3 normal;
void main()
{
    pos = in_position;
    uv = in_uv ;
    normal = in_normal;

    gl_Position = view_matrix * vec4(in_position.x, in_position.y, in_position.z, 1.);
}