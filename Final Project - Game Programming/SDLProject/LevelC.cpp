//
//  LevelC.cpp
//  SDLProject
//
//  Created by Varun Pandian on 4/7/25.
//  Copyright © 2025 ctg. All rights reserved.
//

//


#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 40
#define LEVEL_HEIGHT 8

constexpr char SPRITESHEET_FILEPATH[] = "assets/zorowalk.png",
           ENEMY2_FILEPATH[]      = "assets/guzzwalk.png",
           ENEMY3_FILEPATH[]      = "assets/plant.png",
           FONT_FILEPATH[]        = "assets/font1.png";
//
GLuint m_font_texture_id2;



unsigned int LEVEL3_DATA[LEVEL_HEIGHT * LEVEL_WIDTH] =
{

    0, 0, 0, 0, 0, 37, 37, 37, 37, 37, 109, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0,
    0, 0, 0, 0, 0, 37, 37, 37, 37, 37, 109, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0,
    0, 0, 0, 0, 0, 37, 37, 37, 37, 37, 109, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0,
    0, 0, 0, 0, 0, 37, 37, 37, 37, 37, 109, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0,
    0, 0, 0, 0, 0, 163, 163, 163, 163, 163, 163, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0,
    0, 0, 0, 0, 0, 37, 37, 37, 37, 163, 163, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0,
    0, 0, 0, 0, 0, 37, 37, 37, 37, 163, 163, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0,
    0, 0, 0, 0, 0, 37, 37, 37, 37, 163, 163, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 109, 0, 0,

   
};



LevelC::~LevelC()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelC::initialise()
{
    m_game_state.next_scene_id = -1;
    
    m_font_texture_id2 = Utility::load_texture("assets/font1.png");
    

    GLuint map_texture_id = Utility::load_texture("assets/tiles.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL3_DATA, map_texture_id, 1.0f, 18, 21);
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);


    GLuint youwin = Utility::load_texture("assets/sprites.png");
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
    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

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
    m_game_state.player->set_position(glm::vec3(1.0f, -6.0f, 0.0f));
    m_game_state.player->set_positionstart(glm::vec3(31.0f, -4.2f, 0.0f));
    m_game_state.player->set_texture_win(youwin);
    m_game_state.player->set_texture_lose(youlose);




//    m_game_state.player->set_position(glm::vec3(25.0f, -1.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(10.0f);
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY2_FILEPATH);

    
    


    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i] = Entity(
            enemy_texture_id,   // Texture ID
            1.0f,              // Speed
            glm::vec3(0.0f, 0.0f, 0.0f), // Acceleration
            0.0f,              // Jump power (enemies don't jump)
            hariyama_walking_animation, // Animation frames
            0.0f,              // animation time
            4,                 // Animation frames count
            0,                 // Start with "walk right" animation
            4,                 // Animation columns
            8,                 // Animation rows
            2.5f,             // Width
            2.5f,             // Height
            ENEMY              // Entity type
        );
        

    }

    m_game_state.enemies[1].set_scale(glm::vec3(2.0f, 2.0f, 1.0f));
    m_game_state.enemies[1].set_ai_type(GUARD);
    m_game_state.enemies[1].set_ai_state(IDLE);
    m_game_state.enemies[1].set_position(glm::vec3(-38.0f, -6.2f, 0.0f));
    m_game_state.enemies[1].set_positionstart(glm::vec3(-38.0f, -6.2f, 0.0f));
    
    


//    m_game_state.enemies[0].set_ai_type(CHARGER2);
//    m_game_state.enemies[0].set_ai_state(IDLE1);
//    m_game_state.enemies[0].set_position(glm::vec3(8.0f, -7.2f, 0.0f));
//    m_game_state.enemies[0].set_positionstart(glm::vec3(-38.0f, -6.2f, 0.0f));
    m_game_state.enemies[1].set_texture_id(enemy_texture_id);
    

    m_game_state.enemies[3].set_ai_type(GUARD);
    m_game_state.enemies[3].set_ai_state(IDLE1);

    
    m_game_state.enemies[3].set_scale(glm::vec3(6.0f, 6.0f, 0.0f));
    m_game_state.enemies[3].set_ai_type(CHARGER2);
    m_game_state.enemies[3].set_ai_state(IDLE1);
    m_game_state.enemies[3].set_position(glm::vec3(8.0f, -7.2f, 0.0f));
    m_game_state.enemies[3].set_positionstart(glm::vec3(-38.0f, -6.2f, 0.0f));
    

    
    
    
    

    

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    //from mh wilds
    m_game_state.bgm = Mix_LoadMUS("assets/moonsiders1st.mp3");
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
        m_game_state.hurt_sfx = Mix_LoadWAV("assets/heartbeat.mp3");
        m_game_state.win_sfx = Mix_LoadWAV("assets/win.mp3");

}

int message = 20;


void LevelC::update(float delta_time)
{
    GLuint plant_text_id = Utility::load_texture(ENEMY3_FILEPATH);

    
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
    
    if(m_game_state.player->get_position().x >= 1.0f && message == 20)
    {
        message--;
    }
    
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
        
    }
    m_start_screen_position = m_game_state.player->get_position();
    
    if (m_game_state.player->get_next_scene()) m_game_state.next_scene_id = 2;
    
    

    if(m_game_state.player->get_position().x >= 16.0f && m_game_state.player->get_position().x <= 20.0f )
    {
        GLuint treecko_text_id = Utility::load_texture("assets/treecko.png");

        m_game_state.enemies[3].set_scale(glm::vec3(1.0f, 1.0f, 0.0f));
        m_game_state.enemies[3].set_ai_type(GUARD);
        m_game_state.enemies[3].set_ai_state(IDLE1);
        m_game_state.enemies[3].set_position(glm::vec3(19.0f, -2.2f, 0.0f));
        m_game_state.enemies[3].set_texture_id(plant_text_id);
        m_game_state.enemies[3].set_width(0.85f);
        m_game_state.enemies[3].set_height(0.75f);


//        m_game_state.enemies[3].set_positionstart(glm::/*vec3*/(-38.0f, -6.2f, 0.0f));
        
        m_game_state.player->set_texture_id(treecko_text_id);
        
        
        
//        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("the map gets dark"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y + 1.0f, 0.0f));
//        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("and you can't move while"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 1.0f, 0.0f));
//        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("shining your light"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 2.0f, 0.0f));
        
    }
    
    if(m_game_state.player->get_position().x >= 26.0f && m_game_state.player->get_position().x <= 30.0f )
    {
        GLuint treecko_text_id = Utility::load_texture("assets/obstawalk.png");

        
        m_game_state.player->set_texture_id(treecko_text_id);
        m_game_state.player->set_scale(glm::vec3(1.0f, 1.5f, 0.0f));

//        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("the map gets dark"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y + 1.0f, 0.0f));
//        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("and you can't move while"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 1.0f, 0.0f));
//        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("shining your light"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 2.0f, 0.0f));
        
    }
    
    

}

void LevelC::render(ShaderProgram *g_shader_program)
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
    m_game_state.player->render(g_shader_program);
    for (int i = 0; i < ENEMY_COUNT; i++)
        m_game_state.enemies[i].render(g_shader_program);
    
    if(m_game_state.player->get_position().x >= 1.0f && m_game_state.player->get_position().x <= 2.0f )
    {
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("hey."), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-.5, m_game_state.player->get_position().y + 1.0f, 0.0f));

    }
    if(m_game_state.player->get_position().x > 2.0f && m_game_state.player->get_position().x <= 3.0f )
    {
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("hold up wait"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-2, m_game_state.player->get_position().y + 1.0f, 0.0f));
    }
    
    if(m_game_state.player->get_position().x > 3.0f && m_game_state.player->get_position().x <= 4.0f )
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("press 1 or 2"), 1.0f, -0.75f, glm::vec3(m_game_state.player->get_position().x-2, m_game_state.player->get_position().y + 1.0f, 0.0f));
    
    if(m_game_state.player->get_position().x >= 12.0f && m_game_state.player->get_position().x <= 14.0f )
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("good"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x, m_game_state.player->get_position().y + 1.0f, 0.0f));
        
    if(m_game_state.player->get_position().x >= 14.0f && m_game_state.player->get_position().x <= 16.0f )
    {
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("the map gets dark"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y + 1.0f, 0.0f));
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("and you can't move while"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 1.0f, 0.0f));
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("shining your light"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 2.0f, 0.0f));
        
    }
    
    if(m_game_state.player->get_position().x >= 16.0f && m_game_state.player->get_position().x <= 20.0f )
    {
        
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("press 1 to cut trees"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y + 1.0f, 0.0f));
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("as treecko"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 1.0f, 0.0f));
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("beware of sudowoodo"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 2.0f, 0.0f));
        
    }
    
    
    if(m_game_state.player->get_position().x >= 26.0f && m_game_state.player->get_position().x <= 30.0f )
    {
        
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("press 2 to scare"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y + 1.0f, 0.0f));
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("ghost types as obstagoon"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 1.0f, 0.0f));
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("sword forme aegislash"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 2.0f, 0.0f));
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("aren't afraid though"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 3.0f, 0.0f));
        
    }
        
    
    if(m_game_state.player->get_position().x >= 30.0f && m_game_state.player->get_position().x <= 38.0f )
    {
        
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("illusions break after 1 hit"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4.5, m_game_state.player->get_position().y + 1.0f, 0.0f));
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("so don't fail"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 1.0f, 0.0f));
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("one chance and thats it"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 2.0f, 0.0f));
        Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("good luck"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-4, m_game_state.player->get_position().y - 3.0f, 0.0f));
        
    }
        

//    Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("hey!"), 1.0f, -0.65f, glm::vec3(m_game_state.player->get_position().x-.5, m_game_state.player->get_position().y + 1.0f, 0.0f));
    
   
//    Utility::draw_text(g_shader_program, m_font_texture_id2, std::string("Press Enter to Start"), 1.0f, -0.65f, glm::vec3(-3.0f, 0.0f, 0.0f));
   

   
}

