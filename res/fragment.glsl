#version 410

out vec4 out_color;
uniform vec4 output_color;
uniform float time;
void main()
{
    
    out_color = vec4(output_color);
}