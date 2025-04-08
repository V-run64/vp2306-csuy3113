//
//  Start.h
//  SDLProject
//
//  Created by Varun Pandian on 4/5/25.
//  Copyright © 2025 ctg. All rights reserved.
//


#include "Scene.h"
#include "Utility.h"

class Start : public Scene {
public:
    int ENEMY_COUNT = 0;
    
    ~Start();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
    
};
