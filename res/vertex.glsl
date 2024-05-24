#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
vec2 pos = vec2(in_position.x + .4f, in_position.y + .4f);
uniform float screen_ratio;
uniform float time;
uniform mat4 view_matrix;
out vec2 uv;
out vec3 vertex_position;
void main()
{
    vertex_position = in_position;
    
    //vec2 pos = vec2(in_position.x  + cos(time) / 2, in_position.y + sin(time) / 2);
    //pos.x /= screen_ratio;
    //gl_Position = view_matrix * vec4(pos, 0., 1.);
    uv = in_uv;
    gl_Position = view_matrix * vec4(in_position.x, in_position.y, in_position.z, 1.);
}