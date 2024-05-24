#version 410

out vec4 out_color;
uniform vec4 output_color;
uniform float time;
uniform sampler2D my_texture;
in vec3 vertex_position;
in vec2 uv;
void main()
{
    vec4 texture_color = texture(my_texture, uv);
    out_color = texture_color;
    
    // ***** circle *****
    //float uv_distance = distance(uv , vec2(.5)) * 2;
    //uv_distance = step(.5, uv_distance);
    //out_color = vec4(uv_distance, uv_distance, uv_distance, 1);

    // ***** cadrillage *****
    //vec2 final = step(vec2(.5), fract(uv * 10));

    //vec3 output_vector = (final == vec2(1, 1) || final == vec2(0, 0)) ?  vec3(0, 0, 0) :  vec3(1, 1, 1);
    //out_color = vec4(output_vector, 1);
}