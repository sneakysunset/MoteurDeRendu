#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"
int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); 
    gl::maximize_window(); 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
    auto camera = gl::Camera();
    gl::set_events_callbacks({camera.events_callbacks()});
    glm::mat4 const view_matrix = camera.view_matrix();
    glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);


    auto const shader = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    auto const shaderMask = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/vertexMask.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragmentMask.glsl"},
    }};


    shader.bind(); 

    shader.set_uniform("output_color", glm::vec4(1.f, .78f, .11f, 1.f));
    auto const rectangle_mesh = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::Position2D{1}},
        .data   = {
        -1, -1,  1, 0, 0, //0
         1, -1,  1, 1, 0, //1
        -1,  1,  1, 0, 1, //2
         1,  1,  1, 1, 1,  //3
        -1, -1, -1, 0, 0, //4
         1, -1, -1, 1, 0, //5
        -1,  1, -1, 0, 1, //6
         1,  1, -1, 1, 1  //7
            },
        }},
        .index_buffer = {
        //Top
        2, 6, 7,
        2, 3, 7,

        //Bottom
        0, 4, 5,
        0, 1, 5,

        //Left
        0, 2, 6,
        0, 4, 6,

        //Right
        1, 3, 7,
        1, 5, 7,

        //Front
        0, 2, 3,
        0, 1, 3,

        //Back
        4, 6, 7,
        4, 5, 7
        }
    }};

    auto const rectangle_mask = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position2D{0}},
        .data   = {
                -3.f, -3.f, 
                +3.f, -3.f,
                +3.f, +3.f,
                -3.f, +3.f
            },
        }},
        .index_buffer = {
            0, 1, 2,
            0, 2, 3
        }
    }};

    int i = 0;
    while (gl::window_is_open())
    {
       // i++;
       // if(i%5 == 0){
        //shader.bind(); 
        //shader.set_uniform("time", float(gl::time_in_seconds()));
        //rectangle_mesh.draw(); 
        //shaderMask.bind();
        //rectangle_mask.draw();
        //}
        glm::mat4 const view_matrix = camera.view_matrix();
        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);
        shader.set_uniform("view_matrix", glm::mat4(projection_matrix * view_matrix));
        glClearColor(0.f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.bind();
        shader.set_uniform("screen_ratio", float(gl::framebuffer_aspect_ratio()));
        rectangle_mesh.draw();


    }
}