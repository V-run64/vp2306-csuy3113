//
//  File.cpp
//  SDLProject
//
//  Created by Varun Pandian on 4/5/25.
//  Copyright © 2025 ctg. All rights reserved.
//


#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 40
#define LEVEL_HEIGHT 8

constexpr char SPRITESHEET_FILEPATH[] = "assets/piplupstep.png",
           PLATFORM_FILEPATH[]    = "assets/platformPack_tile027.png",
           ENEMY_FILEPATH[]       = "assets/crabominable_push.png",
           ENEMY2_FILEPATH[]      = "assets/rhyperior walk.png",
           FONT_FILEPATH[]        = "assets/font1.png";


unsigned int LEVEL_DATA[LEVEL_HEIGHT * LEVEL_WIDTH] =
{

    6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 7, 7, 0, 7, 7, 7, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 7, 0, 0, 7, 7, 7, 6, 6, 7, 6, 0, 0, 7, 7, 6, 0, 0, 0, 0, 0, 0, 0, 0,
    6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    6, 7, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 0, 0, 0, 0, 6, 7, 7, 7, 0, 7, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    6, 6, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

   
};

//GLuint text_id = Utility::load_texture(/*FONT_FILEPATH*/);zx


LevelA::~LevelA()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/world_tileset.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA, map_texture_id, 1.0f, 12, 1);
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    GLuint player_jump_texture_id = Utility::load_texture("assets/piplupjump.png");

    m_start_screen_texture_id = Utility::load_texture("assets/bro.jpg");



    int player_walking_animation[4][8] =
    {
        { 24, 25, 26, 27 },  // for piplup to move to the left,
        { 8, 9, 10, 11 }, // for piplup to move to the right,
        { 0, 1, 2, 3 }, // we do not care (up)
        { 0, 4, 8, 12 }   // we do not care (down)
    };
    
  
    
    int hariyama_walking_animation[8][8] =
    {
        { 24, 25, 26, 27 },  // for piplup to move to the left,
        { 8, 9, 10, 11 }, // for piplup to move to the right,
        { 0, 1, 2, 3 }, // we do not care (up)
        { 0, 4, 8, 12 }
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -23.81f, 0.0f);

    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        100.0f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        4,                         // animation column amount
        8,                         // animation row amount
        0.75f,                      // width
        0.45f,                       // height
        PLAYER
    );
    
    //init pos x 5.0f y -4.0f
    m_game_state.player->set_position(glm::vec3(5.0f, -4.0f, 0.0f));
//    m_game_state.player->set_position(glm::vec3(25.0f, -1.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(8.0f);
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);
    GLuint enemy2_texture_id = Utility::load_texture(ENEMY2_FILEPATH);


    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i] = Entity(
            enemy_texture_id,   // Texture ID
            1.0f,              // Speed
            glm::vec3(0.0f, -9.81f * 2, 0.0f), // Acceleration
            0.0f,              // Jump power (enemies don't jump)
            hariyama_walking_animation, // Animation frames
            0.0f,              // animation time
            4,                 // Animation frames count
            0,                 // Start with "walk right" animation
            4,                 // Animation columns
            8,                 // Animation rows 
            0.75f,             // Width
            0.60f,             // Height
            ENEMY              // Entity type
        );
        m_game_state.enemies[i].set_scale(glm::vec3(2.0f, 2.0f, 1.0f));
        m_game_state.enemies[i].set_ai_type(GUARD);
        m_game_state.enemies[i].set_ai_state(IDLE);
    }

   
    m_game_state.enemies[0].set_position(glm::vec3(8.0f, -6.0f, 0.0f));
    m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f*2, 0.0f));
    m_game_state.enemies[1].set_position(glm::vec3(25.0f, -6.2f, 0.0f));
    m_game_state.enemies[1].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[1].set_acceleration(glm::vec3(0.0f, -9.81f * 2, 0.0f));
    m_game_state.enemies[2].set_ai_type(CHARGER);
    m_game_state.enemies[2].set_texture_id(enemy2_texture_id);
    m_game_state.enemies[2].set_position(glm::vec3(37.0f, -1.0f, 0.0f));
  
    


    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/dooblydoo.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(0.0f);
    
    m_game_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
}

void LevelA::update(float delta_time)
{
    
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);

    if (key_state[SDL_SCANCODE_P])
       {
           m_game_started = !m_game_started;
       }

    if (!m_game_started)
       {
           // Check for any key press to start the game
           if ((key_state[SDL_SCANCODE_RETURN] || key_state[SDL_SCANCODE_SPACE]) && !key_state[SDL_SCANCODE_P])
           {
               m_game_started = true;
           }
           return; // Freeze game until started
       }
    
    
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
        
    }
    m_start_screen_position = m_game_state.player->get_position();

}


void LevelA::render(ShaderProgram *g_shader_program)
{
   
    if (!m_game_started)
        {
            // Render only the start screen
            g_shader_program->set_model_matrix(glm::mat4(1.0f));
            
            float vertices[] = { -5.0, -3.75, 5.0, -3.75, 5.0, 3.75, -5.0, -3.75, 5.0, 3.75, -5.0, 3.75 };
            float tex_coords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
            
            glBindTexture(GL_TEXTURE_2D, m_start_screen_texture_id);
            
            glVertexAttribPointer(g_shader_program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
            glEnableVertexAttribArray(g_shader_program->get_position_attribute());
            glVertexAttribPointer(g_shader_program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
            glEnableVertexAttribArray(g_shader_program->get_tex_coordinate_attribute());
            
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisableVertexAttribArray(g_shader_program->get_position_attribute());
            glDisableVertexAttribArray(g_shader_program->get_tex_coordinate_attribute());
            
            return;
        }


//    Utility::draw_text(g_shader_program, text_id, " hi", 16.0f, 0.05f, glm::vec3(1.0f,1.0f,1.0f));

    m_game_state.map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    for (int i = 0; i < ENEMY_COUNT; i++)
            m_game_state.enemies[i].render(g_shader_program);
}


