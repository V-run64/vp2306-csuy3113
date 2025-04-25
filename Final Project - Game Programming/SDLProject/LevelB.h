//
//  LevelB.h
//  SDLProject
//
//  Created by Varun Pandian on 4/5/25.
//  Copyright © 2025 ctg. All rights reserved.
//

#include "Scene.h"

class LevelB : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 10;
//    int ENEMY_COUNT2 = -;
    
    // ————— DESTRUCTOR ————— //
    ~LevelB();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
private:
    bool m_game_started = true;
    GLuint m_start_screen_texture_id;
    glm::vec3 m_start_screen_position;
};

