#version 410

out vec4 out_color;
uniform vec4 output_color;
uniform float time;
in vec3 vertex_position;
in vec2 uv;
void main()
{
    //out_color = vec4(vertex_position, 1) * output_color;
    out_color = vec4(uv.x, uv.y, 0., 1.);
    //out_color = vec4(output_color);
}