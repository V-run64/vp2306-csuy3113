////
////  Start.cpp
////  SDLProject
////
////  Created by Varun Pandian on 4/5/25.
////  Copyright © 2025 ctg. All rights reserved.
////
#include "Start.h"
#include "Utility.h"

unsigned int MENU[] = { 0 };
GLuint m_font_texture_id;

Start::~Start(){
    delete [] m_game_state.enemies;
    delete m_game_state.player;
    delete m_game_state.map;
}

void Start::initialise() {

    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    m_game_state.map = new Map(1, 1, MENU, map_texture_id, 1.0f, 4, 1);
    m_game_state.next_scene_id = -1;

    
    m_font_texture_id = Utility::load_texture("assets/font1.png");
    
  
    
    m_game_state.player = new Entity();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    
    m_game_state.bgm = Mix_LoadMUS("assets/piplupstep.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(0.0f);
    
}

void Start::update(float delta_time) {

    if (m_game_state.player->get_next_scene()) m_game_state.next_scene_id = 1;

}

void Start::render(ShaderProgram *program) {
    m_game_state.map->render(program);
    Utility::draw_text(program, m_font_texture_id, std::string("Piplup Step!!!"), 1.0f, -0.65f, glm::vec3(-2.0f, 2.0f, 0.0f));
    Utility::draw_text(program, m_font_texture_id, std::string("Press Enter to Start"), 1.0f, -0.65f, glm::vec3(-3.0f, 0.0f, 0.0f));
   
}
