/**
* Author: Varun Pandian
* Assignment: Final Project
* Date due: 04/25/25], 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"
#include "Start.h"
#include "PreLevel1.h"
#include "LevelA.h"
#include "LevelB.h"
#include "LevelC.h"
#include "Win.h"
#include "Effects.h"


// ————— CONSTANTS ————— //
constexpr int WINDOW_WIDTH  = 640 * 1.5,
          WINDOW_HEIGHT = 480 * 1.5;

float BG_RED     = 0.1922f,
            BG_BLUE    = 0.549f,
            BG_GREEN   = 0.9059f,
            BG_OPACITY = 1.0f;

constexpr int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;


//,
//            YOUWIN_FILEPATH[] = "assets/youwin.png"
;
//
constexpr char V_SHADER_PATH2[] = "shaders/vertex_lit.glsl",
           F_SHADER_PATH2[] = "shaders/fragment_lit.glsl";


Effects *g_effects;

//GLuint winscreen = Utility::load_texture(YOUWIN_FILEPATH);
constexpr float MILLISECONDS_IN_SECOND = 1000.0;

enum AppStatus { RUNNING, TERMINATED };

// ————— GLOBAL VARIABLES ————— //
Scene *g_current_scene;
Scene *g_levels[4];
Start *g_start;
LevelA *g_level_a;
LevelB *g_level_b;
LevelC *g_level_c;
//Win *g_win;

SDL_Window* g_display_window;

AppStatus g_app_status = RUNNING;
ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;
bool g_check_collision_bottom = false;


void switch_to_scene(Scene *scene)
{
    int player_health = 1;
    if (g_current_scene)player_health = g_current_scene->get_state().player->get_hp();
    g_current_scene = scene;
    g_current_scene->initialise();
    g_current_scene->get_state().player->set_hp(player_health);
    
}

void initialise();
void process_input();
void update();
void render();
void shutdown();


void initialise()
{
    // ————— VIDEO ————— //
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("Poke Maze",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    if (context == nullptr)
    {
        shutdown();
    }
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    // ————— GENERAL ————— //
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_shader_program.load(V_SHADER_PATH2, F_SHADER_PATH2);
    
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    
    g_effects = new Effects(g_projection_matrix, g_view_matrix);
        g_effects->start(FADEIN, 2.0f);

    glUseProgram(g_shader_program.get_program_id());
    
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    
    
    // ————— LEVEL A SETUP ————— //
    g_start = new Start();
    g_level_a = new LevelA();
    g_level_b =new LevelB();
    g_level_c =new LevelC();
//    g_win = new Win();
    
    //put evels in vector
    g_levels[0] = g_start;
    g_levels[2] = g_level_a;
    g_levels[3] = g_level_b;
    g_levels[1] = g_level_c;
//    g_levels[4] = g_win;

    
    

   switch_to_scene(g_levels[0]);

    Mix_Music* bgm = g_current_scene->get_state().bgm;
    if (!bgm) {
        std::cerr << "Failed to load music! Error: " << Mix_GetError() << std::endl;
    } else {
        Mix_PlayMusic(bgm, 4);
        Mix_VolumeMusic(MIX_MAX_VOLUME / 1.0f);
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 1.0f);

    
    // ————— BLENDING ————— //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    
}

void process_input()
{
    g_current_scene->get_state().player->set_movement(glm::vec3(0.0f));
    if (g_current_scene->get_state().player->get_position().x >= 38.5f  && g_current_scene->get_state().player->get_position().y >= -2.0f && g_current_scene != g_level_b)
    {
        printf("lol");

        g_current_scene->get_state().player->set_next_scene(true);
        
    }
    
   
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        
        static Uint32 win_gameover_ticks = 0;
        static bool timer_started = false;

        if (g_current_scene->get_state().player->get_win() ||
            g_current_scene->get_state().player->get_gameover() || g_current_scene->get_state().player->get_hp() == 0)
        {
            g_current_scene->get_state().player->set_movement(glm::vec3(0.0f,0.0f,0.0f));
            g_current_scene->get_state().player->set_acceleration(glm::vec3(0.0f,0.0f,0.0f));
            g_current_scene->get_state().player->set_velocity(glm::vec3(0.0f,0.0f,0.0f));
            
            if(g_current_scene->get_state().player->get_gameover() || g_current_scene->get_state().player->get_hp() == 0)
            {
                Mix_PlayChannel(1,g_current_scene->get_state().hurt_sfx, 0);
            }
            
            if(g_current_scene->get_state().player->get_win())
            {
//                Mix_PlayChannel(1,  g_current_scene->get_state().win_sfx, 0);
//                Mix_VolumeChunk(g_current_scene->get_state().win_sfx, 10);
                Mix_VolumeMusic(MIX_MAX_VOLUME / 20.0f);
            }

            if (!timer_started) {
                win_gameover_ticks = SDL_GetTicks();
                timer_started = true;
            }
            else {
                Uint32 current_ticks = SDL_GetTicks();
                if (current_ticks - win_gameover_ticks >= 5000) {  // 5000ms = 5 seconds
                    g_app_status = TERMINATED;
                    
                }
            }
        }
        
        
        
        else {
            timer_started = false;
        }
        
        // ————— KEYSTROKES ————— //
        switch (event.type) {
            // ————— END GAME ————— //
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_app_status = TERMINATED;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        // Quit the game with a keystroke
                        g_app_status = TERMINATED;
                        break;
                    case SDLK_r:
                        initialise();
                        break;
                        
                    case SDLK_SPACE:
                        // ————— JUMPING ————— //
                        
                    
                        if (g_current_scene->get_state().player->get_collided_bottom())
                        {
                            g_current_scene->get_state().player->jump();
                            Mix_PlayChannel(-1,  g_current_scene->get_state().jump_sfx, 0);
                            Mix_VolumeChunk(g_current_scene->get_state().jump_sfx, 20);
                            
                        }
                         break;
                   
                        
                    case SDLK_RETURN:
                        if (g_current_scene == g_levels[0]) {
                            g_current_scene->render(&g_shader_program);
                            g_current_scene->get_state().player->set_next_scene(true);
                            g_effects->start(FADEIN, 1.0f);
                            g_effects->render();



                        }
                        break;
                        
                    //panic button, skip to the next level
                    case SDLK_n:
                        g_current_scene->render(&g_shader_program);
                           g_effects->render();
                        g_current_scene->get_state().player->set_next_scene(true);
                        
                        break;
                        
                        
                    default:
                        break;
                }
                
            default:
                break;
        }
    }
    
    // ————— KEY HOLD ————— //
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);

    
    if (!(g_current_scene->get_state().player->get_win() ||
         g_current_scene->get_state().player->get_gameover()))
    {
        if(!key_state[SDL_SCANCODE_1] && !key_state[SDL_SCANCODE_2])
        {
            if (key_state[SDL_SCANCODE_LEFT])
            {
                g_current_scene->get_state().player->move_left();
            }
            else if (key_state[SDL_SCANCODE_RIGHT])
            {
                g_current_scene->get_state().player->move_right();
            }
            
            if (key_state[SDL_SCANCODE_UP])
            {
                g_current_scene->get_state().player->move_up();
            }
            else if (key_state[SDL_SCANCODE_DOWN])
            {
                g_current_scene->get_state().player->move_down();
            }
        }
        
        if (key_state[SDL_SCANCODE_1] && (g_current_scene == g_level_a || g_current_scene == g_level_c))
        {
            g_current_scene->get_state().player->attack();
            Mix_PlayChannel(-1,  g_current_scene->get_state().hurt_sfx, 0);
            Mix_VolumeChunk(g_current_scene->get_state().hurt_sfx, 20);


        }
        
        else if (key_state[SDL_SCANCODE_2]  && (g_current_scene == g_level_b || g_current_scene == g_level_c))
        {
            g_current_scene->get_state().player->scare();
            Mix_PlayChannel(-1,  g_current_scene->get_state().hurt_sfx, 0);
            Mix_VolumeChunk(g_current_scene->get_state().hurt_sfx, 20);


        }
        else if (key_state[SDL_SCANCODE_3])
        {
            g_current_scene->get_state().player->hold();

        }
       
        else{
            g_current_scene->get_state().player->scare_off();
        }
        
         
        if (glm::length( g_current_scene->get_state().player->get_movement()) > 1.0f)
            g_current_scene->get_state().player->normalise_movement();
     
    }
    
}
int fade =11;
void update()
{
    // ————— DELTA TIME / FIXED TIME STEP CALCULATION ————— //
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;
    
    
    delta_time += g_accumulator;
    
    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }
    
    while (delta_time >= FIXED_TIMESTEP) {
        // ————— UPDATING THE SCENE (i.e. map, character, enemies...) ————— //
        g_current_scene->update(FIXED_TIMESTEP);
        g_effects->update(FIXED_TIMESTEP);
        g_check_collision_bottom = g_current_scene->get_state().player->get_collided_bottom();
        delta_time -= FIXED_TIMESTEP;
    }
    
    
    g_accumulator = delta_time;
    
    

        
    
    if (g_current_scene->get_state().player->get_position().x >= 38.5f  && g_current_scene->get_state().player->get_position().y >= -2.0f && g_current_scene == g_level_b)
    {
        
        printf("yoooo");
        BG_RED = 0.0f;
        BG_BLUE = 0.0f;
        BG_GREEN = 0.0f;
        glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
        g_current_scene->get_state().player->set_win();
        g_current_scene->render(&g_shader_program);
        g_effects->start(FADEIN, 1.0f);
        g_effects->render();
        Mix_PlayChannel(1,g_current_scene->get_state().win_sfx, 0);
       


    }
    
    
    
    if (g_current_scene->get_state().player->get_position().y < -8.0f)
    {
        printf("yoooo");
        g_current_scene->render(&g_shader_program);
           g_effects->render();
        g_current_scene->get_state().player->die();

        g_current_scene->render(&g_shader_program);
        g_effects->start(FADEIN, 1.0f);
           g_effects->render();
        Mix_PlayChannel(1,g_current_scene->get_state().hurt_sfx, 0);
        

    }
    
    
    
    // ————— PLAYER CAMERA ————— //
    g_view_matrix = glm::mat4(1.0f);
    
    
    if (g_current_scene == g_levels[0]) return;

    if (g_current_scene->get_state().player->get_position().x > LEVEL1_LEFT_EDGE) {
        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->get_state().player->get_position().x, 3.75, 0));
    } else {
        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-5, 3.75, 0));
    }
    
    if(g_current_scene == g_level_a)
    {
        if(g_current_scene->get_state().player->get_position().x>= 27.0f && (fade == 11 || fade == 7 || fade == 3))
        {
            g_effects->start(FADEIN, 1.0f);
            fade--;
            Mix_PlayChannel(-1,  g_current_scene->get_state().jump_sfx, 0);
            Mix_VolumeChunk(g_current_scene->get_state().jump_sfx, 50);
        }
        
        if(g_current_scene->get_state().player->get_position().x>= 29.0f && (fade == 10 || fade == 6 || fade == 2))
        {
            g_effects->start(FADEIN, 1.0f);
            fade--;
            Mix_PlayChannel(-1,  g_current_scene->get_state().jump_sfx, 0);
            Mix_VolumeChunk(g_current_scene->get_state().jump_sfx, 50);
        }
        
        if(g_current_scene->get_state().player->get_position().x>= 31.0f && (fade == 9 || fade == 5 || fade == 1))
        {
            g_effects->start(FADEIN, 1.0f);
            fade--;
            Mix_PlayChannel(-1,  g_current_scene->get_state().jump_sfx, 0);
            Mix_VolumeChunk(g_current_scene->get_state().jump_sfx, 50);
        }
        
        if(g_current_scene->get_state().player->get_position().x>= 33.0f && (fade == 8 || fade == 4 || fade == 0))
        {
            g_effects->start(FADEIN, 1.0f);
            fade--;
            Mix_PlayChannel(-1,  g_current_scene->get_state().jump_sfx, 0);
            Mix_VolumeChunk(g_current_scene->get_state().jump_sfx, 50);
        }
        
    }

    
}

void render()
{
    
    g_shader_program.set_view_matrix(g_view_matrix);
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);

    if(key_state[SDL_SCANCODE_2] || key_state[SDL_SCANCODE_1]  || g_current_scene == g_start)
    {
        g_shader_program.set_light_position_matrix(g_current_scene->get_state().player->get_position());

    }
    else
    {
        
            g_shader_program.set_light_position_matrix(g_current_scene->get_state().enemies[1].get_position());
        
//        g_shader_program.set_light_position_matrix(g_current_scene->enemies[]->get_position());
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    // ————— RENDERING THE SCENE (i.e. map, character, enemies...) ————— //
    g_current_scene->render(&g_shader_program);
    g_effects->render();
     
    if (g_current_scene->get_state().player->get_position().y < -8.0f)
    {
        printf("bruh");
        g_current_scene->render(&g_shader_program);
           g_effects->render();
        g_current_scene->render(&g_shader_program);
           g_effects->render();

    }
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{    
    SDL_Quit();
    
    // ————— DELETING LEVEL A DATA (i.e. map, character, enemies...) ————— //
    delete g_start;
    delete g_level_a;
    delete g_level_b;
    delete g_level_c;
//    delete g_win;n
}

// ————— GAME LOOP ————— //
int main(int argc, char* argv[])
{
    initialise();
    
    

    while (g_app_status == RUNNING)
    {
        process_input();
        update();
        if (g_current_scene->get_state().next_scene_id >= 0)
        {
            switch_to_scene(g_levels[g_current_scene->get_state().next_scene_id]);
            render();
        }
        
        
        render();
        
        
        if(g_current_scene->get_state().player->get_win()||g_current_scene->get_state().player->get_gameover())
        {
        }
        
     
    }
    
    shutdown();
    return 0;
}
