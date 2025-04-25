////
////  163.cpp
////  SDLProject
////
////  Created by Varun Pandian on 4/25/25.
////  Copyright © 2025 ctg. All rights reserved.
////
//
//#include "Win.h"
//#include "Utility.h"
//
//unsigned int MENU3[] = { 0 };
//GLuint m_font_texture_id3;
//
//Win::~Win(){
//    delete [] m_game_state.enemies;
////    delete m_game_state.player;
//    delete m_game_state.map;
//}
//
//void Win::initialise() {
//
//    GLuint map_texture_id = Utility::load_texture("assets/world_tileset.png");
//    m_game_state.map = new Map(1, 1, MENU3, map_texture_id, 1.0f, 4, 1);
//    m_game_state.next_scene_id = -1;
//
//    
//    m_font_texture_id3 = Utility::load_texture("assets/font1.png");
//    
//  
//    
// 
//    
//    m_game_state.bgm = Mix_LoadMUS("assets/moonsiders1st.mp3");
//    Mix_PlayMusic(m_game_state.bgm, -1);
//    Mix_VolumeMusic(0.0f);
//    
//}
//
//void Win::update(float delta_time) {
//
//    
//
//}
//
//void Win::render(ShaderProgram *program) {
//    m_game_state.map->render(program);
//    Utility::draw_text(program, m_font_texture_id3, std::string("brah"), 1.2f, -0.65f, glm::vec3(-1.8f, 1.0f, 0.0f));
//    Utility::draw_text(program, m_font_texture_id3, std::string("good job"), 1.0f, -0.65f, glm::vec3(-3.0f, 0.0f, 0.0f));
//   
//}
