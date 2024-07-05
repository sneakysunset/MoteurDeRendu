#version 410

out vec4 out_color;
uniform sampler2D my_texture;
uniform vec3 directionlight_direction;
uniform float directionlight_intensity;
uniform vec3 directionlight_color;
uniform vec3 pointlight_origin;
uniform float pointlight_intensity;
uniform vec3 pointlight_color;
uniform float ambiantlight_intensity;
uniform vec3 ambiantlight_color;
uniform float time;
in vec3 pos;
in vec2 uv;
in vec3 normal;
void main()
{
    vec4 texture_color = texture(my_texture, uv);
    vec3 pointLightValue = (pos - pointlight_origin) / pow(distance(pointlight_origin, pos), 2) * pointlight_intensity ;
    vec3 lightValue = clamp(-dot(directionlight_direction, normalize(normal)) , 0, 1) * directionlight_intensity * directionlight_color + clamp(-dot(pointLightValue, normalize(normal)),0,1) * pointlight_color + ambiantlight_intensity * ambiantlight_color;
    out_color = vec4(texture_color.rgb * lightValue, 1);
}