#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu

int main()
{
    // Initialisation

    gl::init("TPs de Rendering"); 
        auto const triangle_mesh = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position2D{0}},
        .data   = {
                -1.f, -1.f, 
                +1.f, -1.f, 
                0.f, +1.f 
            },
        }},
    }};
    gl::maximize_window(); 

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        gl::bind_default_shader(); 
        triangle_mesh.draw(); 
    }
}