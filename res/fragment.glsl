#version 410

out vec4 out_color;
uniform sampler2D my_texture;
uniform vec3 directionlight_direction;
uniform float directionlight_intensity;
uniform vec3 pointlight_origin;
uniform float pointlight_intensity;
uniform float ambiantlight_intensity;
in vec3 pos;
in vec2 uv;
in vec3 normal;
void main()
{
    vec4 texture_color = texture(my_texture, uv);
    vec3 pointLightValue = (pos - pointlight_origin) / pow(distance(pointlight_origin, pos), 2) * pointlight_intensity;
    float lightValue = clamp(-dot(directionlight_direction, normalize(normal)) , 0, 1) * directionlight_intensity + clamp(-dot(pointLightValue, normalize(normal)),0,1) + ambiantlight_intensity;
    out_color = vec4(texture_color.rgb * lightValue, 1);
}