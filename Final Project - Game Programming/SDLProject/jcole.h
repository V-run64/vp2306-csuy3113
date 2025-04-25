//
//  LevelB.cpp
//  SDLProject
//
//  Created by Varun Pandian on 4/5/25.
//  Copyright © 2025 ctg. All rights reserved.
//

#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 40
#define LEVEL_HEIGHT 8

constexpr char SPRITESHEET_FILEPATH[] = "assets/obstawalk.png",
           ENEMY_FILEPATH[]       = "assets/mimikw.png",
           ENEMY2_FILEPATH[]      = "assets/rhyperior walk.png",
           ENEMY3_FILEPATH[]      = "assets/s.png",
           FONT_FILEPATH[]        = "assets/font1.png";
//

unsigned int LEVEL_DATA[LEVEL_HEIGHT * LEVEL_WIDTH] =
{

    
     18, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
     18, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
     18, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
     18, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
     18, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
     18, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
     36, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
     54, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55,


   
};

unsigned int LEVEL_OBSDATA[LEVEL_HEIGHT * LEVEL_WIDTH] =
{
    41, 0, 40, 41, 40, 41, 49, 41, 92, 92, 0, 0, 58, 58, 58, 58, 58, 58, 58, 58, 38, 39, 38, 39, 92, 92, 36, 37, 38, 39, 92, 92, 36, 37, 92, 92, 38, 39, 0, 0,
    41, 0, 36, 37, 40, 41, 40, 41, 92, 92, 92, 92, 67, 67, 67, 67, 67, 67, 67, 67, 0, 0, 0, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    41, 0, 0, 0, 0, 27, 28, 29, 92, 92, 92, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 92, 0, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
    41, 0, 38, 39, 0, 0, 0, 0, 92, 92, 92, 92, 0, 36, 37, 36, 37, 36, 37, 36, 37, 0, 92, 92, 0, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    41, 0, 40, 41, 36, 37, 0, 92, 92, 92, 92, 92, 0, 92, 0, 0, 0, 0, 0, 0, 92, 0, 92, 92, 0, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    41, 0, 9, 10, 11, 0, 0, 0, 92, 92, 92, 92, 0, 92, 0, 0, 0, 0, 0, 0, 92, 0, 92, 92, 0, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    41, 0, 18, 19, 39, 36, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 92, 0, 0, 0, 0, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    41, 100, 27, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};




LevelA::~LevelA()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    delete    m_game_state.obs;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::initialise()
{
    m_game_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/tiles.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA, map_texture_id, 1.0f, 18, 16);
    GLuint obs_texture_id = Utility::load_texture("assets/assets.png");
    m_game_state.obs = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_OBSDATA, obs_texture_id, 1.0f, 9, 14);
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    GLuint key2_text_id = Utility::load_texture("assets/bruh.png");
    GLuint k_sh_text_id = Utility::load_texture("assets/kingshieldwalk.png");

    GLuint youlose = Utility::load_texture("assets/loses.png");
    m_start_screen_texture_id = Utility::load_texture("assets/bro.jpg");



    int player_walking_animation[4][8] =
    {
        { 24, 25, 26, 27 },  // for piplup to move to the left,
        { 8, 9, 10, 11 }, // for piplup to move to the right,
        { 16, 17, 18, 19 }, // we do not care (up)
        { 0, 1, 2, 3 }, //DOWN
    };
    
  
    
    int hariyama_walking_animation[8][8] =
    {
        {24, 25, 26, 27 },  // for piplup to move to the left,
       { 8, 9, 10, 11 }, // for piplup to move to the right,
       { 16, 17, 18, 19 }, // we do not care (up)
       { 0, 1, 2, 3 },
    };
    
    int aegislash[8][8]=
    {
        {48,49,50,51,52,53,54,55},
        {16,17,18,19,20,21,22,23},
        {32,33,34,35,36,37,38,39},
        {0,1,2,3,4,5,6,7},
    };

    int tree[8][8] =
    {
        { 0, 0, 0, 0 },  // for piplup to move to the left,
        { 0, 0, 0,0 }, // for piplup to move to the right,
        { 0, 0, 0, 0 }, // we do not care (up)
        { 0, 0, 0, 0 }, //DOWN
        { 12, 13, 14, 15} //NE
    };
    glm::vec3 acceleration = glm::vec3(0.0f, -0.0f, 0.0f);

    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        .85f,                      // speed
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
    m_game_state.player->set_position(glm::vec3(1.0f, -5.0f, 0.0f));
    m_game_state.player->set_scale(glm::vec3(1.0f, 1.5f, 0.0f));

    m_game_state.player->set_positionstart(glm::vec3(1.0f, -5.0f, 0.0f));
    m_game_state.player->set_texture_lose(youlose);

//    m_game_state.player->set_position(glm::vec3(25.0f, -1.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(8.0f);
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);
    GLuint enemy2_texture_id = Utility::load_texture(ENEMY2_FILEPATH);
    GLuint enemy3_texture_id = Utility::load_texture(ENEMY3_FILEPATH);
    


    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i] = Entity(
            enemy_texture_id,   // Texture ID
            1.0f,              // Speed
            glm::vec3(0.0f, 0 * 2, 0.0f), // Acceleration
            0.0f,              // Jump power (enemies don't jump)
            hariyama_walking_animation, // Animation frames
            0.0f,              // animation time
            4,                 // Animation frames count
            0,                 // Start with "walk right" animation
            4,                 // Animation columns
            8,                 // Animation rows
            0.75f,             // Width
            0.85f,             // Height
            ENEMY              // Entity type
        );
        m_game_state.enemies[i].set_scale(glm::vec3(2.0f, 2.0f, 1.0f));
        m_game_state.enemies[i].set_ai_type(GUARD);
        m_game_state.enemies[i].set_ai_state(IDLE);
        
    }

    
    //i know how to wrtie loops i just had an exam and want to brute force this my bad
   
    m_game_state.enemies[0].set_position(glm::vec3(5.0f, -3.0f, 0.0f));
    m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[0].set_velocity(glm::vec3(2.0f, 0.0f, 0.0f));
    m_game_state.enemies[0].set_texture_id(k_sh_text_id);
    m_game_state.enemies[0].set_animation_cols(8);
    m_game_state.enemies[0].set_walking2(aegislash);
    m_game_state.enemies[0].set_ai_type(CHARGER2);
    m_game_state.enemies[0].set_ai_state(IDLE2);
    
//    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f*2, 0.0f));
    m_game_state.enemies[1].set_texture_id(enemy_texture_id);
    m_game_state.enemies[1].set_position(glm::vec3(1.0, 2.0f, 0.0f));
    m_game_state.enemies[1].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[1].set_ai_state(WALKING2);
    m_game_state.enemies[1].set_scale(glm::vec3(2.0f, 2.0f, 0.0f));
    m_game_state.enemies[2].set_ai_type(CHARGER2);
    m_game_state.enemies[2].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[2].set_position(glm::vec3(23.0f, -4.2f, 0.0f));
    m_game_state.enemies[2].set_texture_id(enemy3_texture_id);
    m_game_state.enemies[2].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    m_game_state.enemies[3].set_ai_type(CHARGER2);
    m_game_state.enemies[3].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[3].set_position(glm::vec3(23.0f, -2.2f, 0.0f));
    m_game_state.enemies[3].set_texture_id(enemy3_texture_id);
    m_game_state.enemies[3].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    m_game_state.enemies[3].set_ai_type(CHARGER2);
    m_game_state.enemies[3].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[3].set_position(glm::vec3(23.0f, -5.2f, 0.0f));
    m_game_state.enemies[3].set_texture_id(enemy3_texture_id);
    m_game_state.enemies[3].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    m_game_state.enemies[4].set_ai_type(CHARGER);
    m_game_state.enemies[4].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[4].set_position(glm::vec3(33.0f, -3.2f, 0.0f));
    m_game_state.enemies[4].set_texture_id(enemy3_texture_id);
    m_game_state.enemies[4].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    m_game_state.enemies[5].set_ai_type(GUARD);
    m_game_state.enemies[5].set_ai_state(IDLE2);
    m_game_state.enemies[5].set_scale(glm::vec3(0.5f, 0.5f, 0.0f));
    m_game_state.enemies[5].set_position(glm::vec3(1.0f, -7.f, 0.0f));
    m_game_state.enemies[5].set_texture_id(key2_text_id);
    m_game_state.enemies[5].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));





    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/shadowsdancing.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(0.0f);
    
     
    Mix_Music* bgm = m_game_state.bgm;
    if (!bgm) {
        std::cerr << "Failed to load music! Error: " << Mix_GetError() << std::endl;
    } else {
        Mix_PlayMusic(bgm, 4);
        Mix_VolumeMusic(MIX_MAX_VOLUME / 1.0f);
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 1.0f);

    //  Sound Effect by <a href="https://pixabay.com/users/driken5482-45721595/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=236683">Driken Stan</a> from <a href="https://pixabay.com/sound-effects//?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=236683">Pixabay</a>

        m_game_state.jump_sfx = Mix_LoadWAV("assets/jump.mp3");
        m_game_state.hurt_sfx = Mix_LoadWAV("assets/lobotomy.mp3");
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
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.obs);

    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
//        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.obs);

        
    }
    m_start_screen_position = m_game_state.player->get_position();
    if (m_game_state.player->get_next_scene()) m_game_state.next_scene_id = 3;


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



    m_game_state.map->render(g_shader_program);
    m_game_state.obs->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    for (int i = 0; i < ENEMY_COUNT; i++)
        m_game_state.enemies[i].render(g_shader_program);
}

