//
//  LevelC.h
//  SDLProject
//
//  Created by Varun Pandian on 4/7/25.
//  Copyright © 2025 ctg. All rights reserved.
//

#include "Scene.h"

class LevelC : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 4;
//    int ENEMY_COUNT2 = -;
    
    // ————— DESTRUCTOR ————— //
    ~LevelC();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
private:
    bool m_game_started = true;
    GLuint m_start_screen_texture_id;
    glm::vec3 m_start_screen_position;
};

