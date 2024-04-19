#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); 
    //gl::maximize_window(); 
    
    auto const shader = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    auto const rectangle_mesh = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position2D{0}},
        .data   = {
                -0.5f, -0.5f, 
                +0.5f, -0.5f,
                +0.5f, +0.5f,
                -0.5f, +0.5f
            },
        }},
        .index_buffer = {
            0, 1, 2,
            0, 2, 3
        }
    }};



    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.bind(); 
        rectangle_mesh.draw(); 
    }
}