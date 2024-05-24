#version 410

out vec4 out_color;
uniform sampler2D my_texture;
in vec2 uv;
void main()
{
    vec4 texture_color = texture(my_texture, uv);
    out_color = texture_color;
}