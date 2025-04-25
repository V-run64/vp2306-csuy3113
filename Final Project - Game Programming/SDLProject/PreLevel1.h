//
//  PreLevel1.h
//  SDLProject
//
//  Created by Varun Pandian on 4/25/25.
//  Copyright © 2025 ctg. All rights reserved.
//


#include "Scene.h"
#include "Utility.h"

class PreLevel1 : public Scene {
public:
    int ENEMY_COUNT = 0;
    
    ~PreLevel1();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
    
};
