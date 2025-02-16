
/**
* Author: Varun Pandian vp2306
* Assignment: Simple 2D Scene
* Date due: 2025-02-15, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

//i dont know why but sometimes magnamalo doesnt pop up on first launch, he shows up
//every other time though

#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#define LOG(argument) std::cout << argument << '\n'
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "cmath"
#include <ctime>
enum AppStatus { RUNNING, TERMINATED };
constexpr float WINDOW_SIZE_MULT = 2.0f;
constexpr int WINDOW_WIDTH  = 640,
              WINDOW_HEIGHT = 480;
constexpr float BG_RED     = 0.9765625f,
                BG_GREEN   = 0.97265625f,
                BG_BLUE    = 0.9609375f,
                BG_OPACITY = 1.0f;
constexpr int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;
constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";
constexpr GLint NUMBER_OF_TEXTURES = 1;
constexpr GLint LEVEL_OF_DETAIL    = 0;
constexpr GLint TEXTURE_BORDER     = 0;
constexpr float MILLISECONDS_IN_SECOND = 1000.0;
//courtesy of Capcom
constexpr char MAGNAMALO_SPRITE_FILEPATH[] = "magnamalo.png",
               REY_DAU_SPRITE_FILEPATH[]  = "rey dau.png";
constexpr float MINIMUM_COLLISION_DISTANCE = 1.0f;
constexpr glm::vec3 INIT_SCALE_MAGNAMALO = glm::vec3(2.75f, 2.75f, 2.75f),
                    INIT_POS_MAGNAMALO   = glm::vec3(3.0f, 2.0f, 0.0f),
                    INIT_SCALE_REYDAU  = glm::vec3(3.0f, 3.0f, 3.0f),
                    INIT_POS_REYDAU    = glm::vec3(-3.0f, -2.0f, 0.0f);
                    
SDL_Window* g_display_window;
AppStatus g_app_status = RUNNING;
ShaderProgram g_shader_program = ShaderProgram();
glm::mat4 g_view_matrix, g_magna_matrix, g_projection_matrix, g_rey_matrix;
float g_previous_ticks = 0.0f;
GLuint g_magna_texture_id;
GLuint g_rey_texture_id;
glm::vec3 g_magna_position   = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_magna_movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_rey_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_rey_movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_rey_scale = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_rey_size  = glm::vec3(1.0f, 1.0f, 0.0f);
glm::vec3 g_rotation_magna    = glm::vec3(0.0f, 0.0f, 0.0f),
          g_rotation_rey = glm::vec3(0.0f, 0.0f, 0.0f);
constexpr float ROT_INCREMENT = 1.0f;
void initialise();
void process_input();
void update();
void render();
void shutdown();
GLuint load_texture(const char* filepath)
{
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);
    if (image == NULL)
    {
        LOG("Unable to load image. Make sure the path is correct.");
        assert(false);
    }
    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}
void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("Project 1 - Varun Pandian",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    if (g_display_window == nullptr) shutdown();
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    g_magna_matrix = glm::mat4(1.0f);
    g_rey_matrix = glm::mat4(1.0f);
    g_rey_matrix = glm::translate(g_rey_matrix, glm::vec3(1.0f, 1.0f, 0.0f));
    g_rey_position += g_rey_movement;
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    glUseProgram(g_shader_program.get_program_id());
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    g_magna_texture_id = load_texture(MAGNAMALO_SPRITE_FILEPATH);
    g_rey_texture_id = load_texture(REY_DAU_SPRITE_FILEPATH);
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void process_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            g_app_status = TERMINATED;
        }
    }
}
void update()
{
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;
    
    
    //diagonal movement
    g_magna_position += glm::vec3(1.0f, 1.0f, 0.0f) * delta_time;
    static float time_flip = 0.0f;
    constexpr float interval = 1.0f;
    
    time_flip += delta_time;
    
    //i wasn't sure how to actually get him to reverse position
    //but this was my solution to keeping him on screen
    if(time_flip >= interval)
    {
        g_magna_position *= -1.0f;
        time_flip = 0.0f;
    }
   
    constexpr float CIRCLE_RADIUS = 2.0f;
    constexpr float CIRCLE_SPEED = 2.0f;
    static float g_angle = 0.0f;
    
    
    //circular movement
    g_angle += CIRCLE_SPEED * delta_time;
    g_rey_position.x = INIT_POS_REYDAU.x + CIRCLE_RADIUS * cos(g_angle);
    g_rey_position.y = INIT_POS_REYDAU.y + CIRCLE_RADIUS * sin(g_angle);
    
    //sick flips cat
    g_rotation_magna.x += ROT_INCREMENT * delta_time * 10;
    g_rotation_magna.y += ROT_INCREMENT * delta_time * -5;
    
    g_rotation_rey.z += ROT_INCREMENT * delta_time * 5;
    
    
    //https://tenor.com/view/rey-dau-approaching-monster-hunter-gif-15525627856799099580
    g_rey_scale += glm::vec3(0.5f, 0.5f, 0.5f) * delta_time;
    
   
    g_magna_matrix = glm::mat4(1.0f);
    g_magna_matrix = glm::translate(g_magna_matrix, g_magna_position);
    g_magna_matrix = glm::rotate(g_magna_matrix, g_rotation_magna.x, glm::vec3(1.0f, 0.0f, 0.0f));
    g_magna_matrix = glm::rotate(g_magna_matrix, g_rotation_magna.y, glm::vec3(0.0f, 1.0f, 1.0f));
    g_magna_matrix = glm::scale(g_magna_matrix, INIT_SCALE_MAGNAMALO);
    g_rey_matrix = glm::mat4(1.0f);
    g_rey_matrix = glm::translate(g_rey_matrix, g_rey_position);
    g_rey_matrix = glm::rotate(g_rey_matrix, g_rotation_rey.z, glm::vec3(0.0f, 0.0f, 1.0f));
    g_rey_matrix = glm::scale(g_rey_matrix, g_rey_scale);
}
void draw_object(glm::mat4 &object_model_matrix, GLuint &object_texture_id)
{
    g_shader_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    float vertices[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f
    };
    float texture_coordinates[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };
    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    draw_object(g_magna_matrix, g_magna_texture_id);
    draw_object(g_rey_matrix, g_rey_texture_id);
    
    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    SDL_GL_SwapWindow(g_display_window);
}
void shutdown() { SDL_Quit(); }
int main(int argc, char* argv[])
{
    initialise();
    while (g_app_status == RUNNING)
    {
        process_input();
        update();
        render();
    }
    shutdown();
    return 0;
}

