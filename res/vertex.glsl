#version 410

layout(location = 0) in vec2 in_position;
vec2 pos = vec2(in_position.x + .4f, in_position.y + .4f);
void main()
{
   // vec2 pos = vec2(in_position.x + .4f, in_position.y + .4f);
    gl_Position = vec4(pos, 0., 1.);
}