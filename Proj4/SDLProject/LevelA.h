#include "Scene.h"

class LevelA : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 4;
//    int ENEMY_COUNT2 = -;
    
    // ————— DESTRUCTOR ————— //
    ~LevelA();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
private:
    Entity m_goal;
    bool m_game_started = true;
    GLuint m_start_screen_texture_id;
    glm::vec3 m_start_screen_position;
};

