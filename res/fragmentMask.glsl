#version 410

out vec4 out_color;
uniform sampler2D my_texture;
uniform float time;
in vec2 uv;
void main()
{
    vec2 newuv = uv;

    // ***** deformation *****
    //newuv.y += sin(newuv.x * 10.0 + time) * 0.1;
    
    vec4 texture_color = texture(my_texture, newuv);

    // ***** contrast *****
    //float gray_value = (texture_color.x + texture_color.y + texture_color.z) / 3;
    //float contrast_value = gray_value > .5 ?  1.3 :  .7;
    //texture_color = vec4(texture_color.rgb * contrast_value, 1);

    // ***** saturation *****
    //float gray_value = (texture_color.x + texture_color.y + texture_color.z) / 3;
    //texture_color = vec4(mix(vec3(gray_value), texture_color.rgb, .3), 1);
    
    // ***** vignettage *****
    //if(uv.x > .9 || uv.y > .9 || uv.x < .1 || uv.y < .1){
    //    texture_color = vec4(0.0, 0.0, 0.0, 1.0);
    //}

    out_color = texture_color;
}