
#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"
#include "../opengl-framework/lib/glfw/include/GLFW/glfw3.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

void Initialisation()
{
    gl::init("TPs de Rendering");
    gl::maximize_window();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);

}

gl::Mesh LoadMesh(std::string inputFile) {

    std::string inputfile = inputFile;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./";
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }


    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();


    std::vector<float> vertices;

    // Extract the vertices and indices from the loaded OBJ file
    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                tinyobj::real_t ux = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                tinyobj::real_t uy = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                tinyobj::real_t normalx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                tinyobj::real_t normaly = attrib.normals[3 * size_t(idx.normal_index) + 1];
                tinyobj::real_t normalz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);
                vertices.push_back(ux);
                vertices.push_back(uy);
                vertices.push_back(normalx);
                vertices.push_back(normaly);
                vertices.push_back(normalz);

            }
            index_offset += fv;
        }
    }

    return gl::Mesh{ {
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::Position2D{1}, gl::VertexAttribute::Position3D{2}},
            .data = vertices,
        }},
    } };
}

gl::Mesh LoadCube(float scale, glm::vec3 pos_offset) 
{
    return gl::Mesh{ {
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::Position2D{1}},
        .data = {
            // Front face                                        
            -scale + pos_offset.x, -scale + pos_offset.y,  scale + pos_offset.z, 0, 0, // 0
             scale + pos_offset.x, -scale + pos_offset.y,  scale + pos_offset.z, 1, 0, // 1
            -scale + pos_offset.x,  scale + pos_offset.y,  scale + pos_offset.z, 0, 1, // 2
             scale + pos_offset.x,  scale + pos_offset.y,  scale + pos_offset.z, 1, 1, // 3

             // Back face
             -scale + pos_offset.x, -scale + pos_offset.y, -scale + pos_offset.z, 0, 0, // 4
              scale + pos_offset.x, -scale + pos_offset.y, -scale + pos_offset.z, 1, 0, // 5
             -scale + pos_offset.x,  scale + pos_offset.y, -scale + pos_offset.z, 0, 1, // 6
              scale + pos_offset.x,  scale + pos_offset.y, -scale + pos_offset.z, 1, 1, // 7

              // Left face
              -scale + pos_offset.x, -scale + pos_offset.y, -scale + pos_offset.z, 0, 0, // 8
              -scale + pos_offset.x, -scale + pos_offset.y,  scale + pos_offset.z, 1, 0, // 9
              -scale + pos_offset.x,  scale + pos_offset.y, -scale + pos_offset.z, 0, 1, // 10
              -scale + pos_offset.x,  scale + pos_offset.y,  scale + pos_offset.z, 1, 1, // 11

              // Right face
               scale + pos_offset.x, -scale + pos_offset.y, -scale + pos_offset.z, 0, 0, // 12
               scale + pos_offset.x, -scale + pos_offset.y,  scale + pos_offset.z, 1, 0, // 13
               scale + pos_offset.x,  scale + pos_offset.y, -scale + pos_offset.z, 0, 1, // 14
               scale + pos_offset.x,  scale + pos_offset.y,  scale + pos_offset.z, 1, 1, // 15

               // Top face
               -scale + pos_offset.x,  scale + pos_offset.y, -scale + pos_offset.z, 0, 0, // 16
                scale + pos_offset.x,  scale + pos_offset.y, -scale + pos_offset.z, 1, 0, // 17
               -scale + pos_offset.x,  scale + pos_offset.y,  scale + pos_offset.z, 0, 1, // 18
                scale + pos_offset.x,  scale + pos_offset.y,  scale + pos_offset.z, 1, 1, // 19

                // Bottom face
                -scale + pos_offset.x, -scale + pos_offset.y, -scale + pos_offset.z, 0, 0, // 20
                 scale + pos_offset.x, -scale + pos_offset.y, -scale + pos_offset.z, 1, 0, // 21
                -scale + pos_offset.x, -scale + pos_offset.y,  scale + pos_offset.z, 0, 1, // 22
                 scale + pos_offset.x, -scale + pos_offset.y,  scale + pos_offset.z, 1, 1, // 23
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
}

int main()
{
    Initialisation();

    auto camera = gl::Camera();
    gl::set_events_callbacks({ camera.events_callbacks() });
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


    auto const shader_camera = gl::Shader{{
     .vertex   = gl::ShaderSource::File{"res/vertexMask.glsl"},
     .fragment = gl::ShaderSource::File{"res/fragmentMask.glsl"},
     }};
    auto const shader = gl::Shader{ {
.vertex = gl::ShaderSource::File{"res/vertex.glsl"},
.fragment = gl::ShaderSource::File{"res/fragment.glsl"},
} };
    auto const texture = gl::Texture
    {
        gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
            .path = "res/Model/fourareen2K_albedo.jpg",
            .flip_y = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
            .texture_format = gl::InternalFormat::RGBA32F, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
        },
        gl::TextureOptions{
            .minification_filter = gl::Filter::Linear, // Comment on va moyenner les pixels quand on voit l'image de loin ?
            .magnification_filter = gl::Filter::Linear, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
            .wrap_x = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
            .wrap_y = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
        }
    };


    shader_camera.bind();
    shader_camera.set_uniform("my_texture", render_target.color_texture(0));
    auto const camera_rect = gl::Mesh{ {
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position2D{0}, gl::VertexAttribute::Position2D{1}},
        .data = {
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
    } };
    glm::vec3 directionlight_direction(.2, .3, -1.);
    float directionlight_intensity = 1;
    glm::vec3 directionlight_color(1, 1, 1);
    glm::vec3 pointlight_origin(0, .6, 0);
    float pointlight_intensity = .7;
    glm::vec3 pointlight_color_0(.1, .1, 1);
    glm::vec3 pointlight_color_1(1, .1, .1);
    float ambiantlight_intensity = .1;
    glm::vec3 ambiantlight_color(1, 1, 1);
    auto mesh = LoadMesh(gl::make_absolute_path("res/Model/fourareen.obj").generic_string());
    shader.bind();
    shader.set_uniform("directionlight_direction", directionlight_direction);
    shader.set_uniform("directionlight_intensity", directionlight_intensity);
    shader.set_uniform("directionlight_color", directionlight_color);
    shader.set_uniform("pointlight_origin", pointlight_origin);
    shader.set_uniform("pointlight_intensity", pointlight_intensity);
    shader.set_uniform("pointlight_color", pointlight_color_0);
    shader.set_uniform("ambiantlight_intensity", ambiantlight_intensity);
    shader.set_uniform("ambiantlight_color", ambiantlight_color);

    auto cube_mesh = LoadCube(.1, pointlight_origin);

    while (gl::window_is_open())
    {
        render_target.render([&]() 
        {
            glm::mat4 const view_matrix = camera.view_matrix();
            glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);
            glClearColor(.5f, .5f, .5f, 1.f); // Dessine du rouge, non pas à l'écran, mais sur notre render target
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shader.bind();
            shader.set_uniform("my_texture", texture);
            glm::mat4 transform_matrix(1.0f);
            transform_matrix = glm::rotate(transform_matrix, float(gl::time_in_seconds() / 3), glm::vec3(1.0f, 0.0f, 0.0f));
            shader.set_uniform("view_matrix", view_matrix);
            shader.set_uniform("transform_matrix", transform_matrix);
            shader.set_uniform("projection_matrix", projection_matrix);
            shader.set_uniform("time", float(gl::time_in_seconds()));
            shader.set_uniform("pointlight_color", glm::mix(pointlight_color_0, pointlight_color_1, glm::sin((float)gl::time_in_seconds())));
            shader.set_uniform("pointlight_intensity", glm::mix((float)0., pointlight_intensity, glm::sin((float)gl::time_in_seconds() * 2)));
            

            mesh.draw();
            //shader.set_uniform("transform_matrix", glm::mat4(1));
            //cube_mesh.draw();
        });

        glClear(GL_DEPTH_BUFFER_BIT);
        shader_camera.bind();
        shader_camera.set_uniform("my_texture", render_target.color_texture(0));
        shader_camera.set_uniform("time", float(gl::time_in_seconds()));
        camera_rect.draw();
    }
}

