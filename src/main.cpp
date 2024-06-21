
#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"
#include "../opengl-framework/lib/glfw/include/GLFW/glfw3.h"


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
    
    auto const shader = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    auto const shader_camera = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/vertexMask.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragmentMask.glsl"},
    }};

    auto const shader_fader = gl::Shader{ {
        .vertex = gl::ShaderSource::File{"res/vertexFader.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragmentFader.glsl"},
        } };

    auto render_target = gl::RenderTarget{ gl::RenderTarget_Descriptor{
    .width = gl::framebuffer_width_in_pixels(),
    .height = gl::framebuffer_height_in_pixels(),
    .color_textures = {
        gl::ColorAttachment_Descriptor{
            .format = gl::InternalFormat_Color::RGBA32F,
            .options = {
                .minification_filter = gl::Filter::NearestNeighbour, // On va toujours afficher la texture à la taille de l'écran,
                .magnification_filter = gl::Filter::NearestNeighbour, // donc les filtres n'auront pas d'effet. Tant qu'à faire on choisit le moins coûteux.
                .wrap_x = gl::Wrap::ClampToEdge,
                .wrap_y = gl::Wrap::ClampToEdge,
            },
        },
    },
    .depth_stencil_texture = gl::DepthStencilAttachment_Descriptor{
        .format = gl::InternalFormat_DepthStencil::Depth32F,
        .options = {
            .minification_filter = gl::Filter::NearestNeighbour,
            .magnification_filter = gl::Filter::NearestNeighbour,
            .wrap_x = gl::Wrap::ClampToEdge,
            .wrap_y = gl::Wrap::ClampToEdge,
        },
    },
} };

    gl::set_events_callbacks({
    camera.events_callbacks(),
    {.on_framebuffer_resized = [&](gl::FramebufferResizedEvent const& e) {
        render_target.resize(e.width_in_pixels, e.height_in_pixels);
    }},
        });

    shader.bind(); 

    auto const cube_mesh = gl::Mesh{ {
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::Position2D{1}},
            .data = {
            // Front face
            -1, -1,  1, 0, 0, // 0
             1, -1,  1, 1, 0, // 1
            -1,  1,  1, 0, 1, // 2
             1,  1,  1, 1, 1, // 3

             // Back face
             -1, -1, -1, 0, 0, // 4
              1, -1, -1, 1, 0, // 5
             -1,  1, -1, 0, 1, // 6
              1,  1, -1, 1, 1, // 7

              // Left face
              -1, -1, -1, 0, 0, // 8
              -1, -1,  1, 1, 0, // 9
              -1,  1, -1, 0, 1, // 10
              -1,  1,  1, 1, 1, // 11

              // Right face
               1, -1, -1, 0, 0, // 12
               1, -1,  1, 1, 0, // 13
               1,  1, -1, 0, 1, // 14
               1,  1,  1, 1, 1, // 15

               // Top face
               -1,  1, -1, 0, 0, // 16
                1,  1, -1, 1, 0, // 17
               -1,  1,  1, 0, 1, // 18
                1,  1,  1, 1, 1, // 19

                // Bottom face
                -1, -1, -1, 0, 0, // 20
                 1, -1, -1, 1, 0, // 21
                -1, -1,  1, 0, 1, // 22
                 1, -1,  1, 1, 1, // 23
                },
            }},
            .index_buffer = {
            // Front face
            0, 1, 2,
            1, 3, 2,

            // Back face
            4, 5, 6,
            5, 7, 6,

            // Left face
            8, 9, 10,
            9, 11, 10,

            // Right face
            12, 13, 14,
            13, 15, 14,

            // Top face
            16, 17, 18,
            17, 19, 18,

            // Bottom face
            20, 21, 22,
            21, 23, 22
        }
    } };


    auto const texture = gl::Texture{
    gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
        .path = "res/texture.png",
        .flip_y = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
        .texture_format = gl::InternalFormat::RGBA8, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
    },
    gl::TextureOptions{
        .minification_filter = gl::Filter::Linear, // Comment on va moyenner les pixels quand on voit l'image de loin ?
        .magnification_filter = gl::Filter::Linear, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
        .wrap_x = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
        .wrap_y = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
    }
    };

    auto const rectangle_mask = gl::Mesh{ {
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0}},
            .data = {
                    -3.f, -3.f,
                    +3.f, -3.f,
                    +3.f, +3.f,
                    -3.f, +3.f
                },}},
        .index_buffer = 
        {
            0, 1, 2,
            0, 2, 3
        }}};
  


    shader_camera.bind();
    shader_camera.set_uniform("my_texture", render_target.color_texture(0));
    auto const camera_rect = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position2D{0}, gl::VertexAttribute::Position2D{1}},
        .data   = {
                -1.f, -1.f, 0, 0, 
                +1.f, -1.f, 1, 0,
                +1.f, +1.f, 1, 1,
                -1.f, +1.f, 0, 1
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
        render_target.render([&]() 
        {
            //glClearColor(.5f, .5f, .5f, 1.f); // Dessine du rouge, non pas à l'écran, mais sur notre render target
            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glm::mat4 const view_matrix = camera.view_matrix();
            glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);
   /*         shader.bind();
            shader.set_uniform("view_matrix", glm::mat4(projection_matrix* view_matrix));
            shader.set_uniform("my_texture", texture);
            cube_mesh.draw();*/

            //Test
            glClear(GL_DEPTH_BUFFER_BIT);
            shader.bind();
            shader.set_uniform("view_matrix", glm::mat4(projection_matrix* view_matrix));
            shader.set_uniform("my_texture", texture);
            shader.set_uniform("time", float(gl::time_in_seconds()));
            cube_mesh.draw();
            shader_fader.bind();
            rectangle_mask.draw();

        });

        //glClearColor(1.f, 0.f, 0.f, 1.f); 
        glClear(/*GL_COLOR_BUFFER_BIT | */GL_DEPTH_BUFFER_BIT);
        shader_camera.bind();
        shader_camera.set_uniform("my_texture", render_target.color_texture(0));
        shader_camera.set_uniform("time", float(gl::time_in_seconds()));
        camera_rect.draw();
    }


}