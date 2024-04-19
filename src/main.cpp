#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); 
    gl::maximize_window(); 
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);

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
    shader.set_uniform("screen_ratio", float(gl::framebuffer_aspect_ratio()));
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
        i++;
        if(i%5 == 0){
        shader.bind(); 
        shader.set_uniform("time", float(gl::time_in_seconds()));
        rectangle_mesh.draw(); 
        shaderMask.bind();
        rectangle_mask.draw();
        }


    }
}