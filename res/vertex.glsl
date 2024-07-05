#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
uniform float time;
uniform mat4 view_matrix;
uniform mat4 transform_matrix;
uniform mat4 projection_matrix;
out vec3 pos;
out vec2 uv;
out vec3 normal;

vec3 apply_matrix_to_position(mat4 matrix, vec3 point)
{
    vec4 tmp = matrix * vec4(point, 1.);
    return tmp.xyz / tmp.w;
}

vec3 apply_matrix_to_direction(mat4 matrix, vec3 direction)
{
    vec4 tmp = matrix * vec4(direction, 0.);
    return normalize(tmp.xyz);
}

void main()
{
    mat4 finalview_matrix =  projection_matrix * view_matrix  * transform_matrix;
    pos = apply_matrix_to_position(transform_matrix, in_position);
    normal = apply_matrix_to_direction(inverse(transpose(transform_matrix)), in_normal);
    uv = in_uv ;

    gl_Position = finalview_matrix * vec4(in_position, 1.);
}
