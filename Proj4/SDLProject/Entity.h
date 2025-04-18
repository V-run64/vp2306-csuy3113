#ifndef ENTITY_H
#define ENTITY_H

#include "Map.h"
#include "glm/glm.hpp"
#include "ShaderProgram.h"
enum EntityType { PLATFORM, PLAYER, ENEMY, GOAL };
enum AIType     { WALKER, GUARD, CHARGER, CHARGER2, BIG_BOSS };
enum AIState    { WALKING, IDLE, ATTACKING, WALKING1, ATTACKING1, IDLE2, IDLE1, WALKING2,
                WALKING3, IDLE3, ATTACKING2, WALKING4 };


enum AnimationDirection { LEFT, RIGHT, UP, DOWN };

class Entity
{
private:
    bool m_is_active = true;
    
    int m_walking[20][4]; // 4x4 array for walking animations

    
    EntityType m_entity_type;
    AIType     m_ai_type;
    AIState    m_ai_state;
    // ————— TRANSFORMATIONS ————— //
    glm::vec3 m_movement;
    glm::vec3 m_position;
    glm::vec3 m_start_pos;
    glm::vec3 m_scale;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;

    glm::mat4 m_model_matrix;

    float     m_speed,
              m_jumping_power;
    
    bool m_is_jumping;
    bool is_next;
    bool is_gameover = false;
    bool is_win = false;

    // ————— TEXTURES ————— //
    GLuint    m_texture_id;
    GLuint    m_win_texture;
    GLuint    m_lose_texture;
    GLuint    m_jump_texture_id;

    // ————— ANIMATION ————— //
    void set_walk_texture_id(GLuint id) { m_texture_id = id; }
    void set_jump_texture_id(GLuint id) { m_jump_texture_id = id; }
    GLuint get_current_texture_id() const {
           return (m_is_jumping) ? m_jump_texture_id : m_texture_id;
       }
    
    
    int m_animation_cols;
    int m_animation_frames,
        m_animation_index,
        m_animation_rows;

    int* m_animation_indices = nullptr;
    float m_animation_time = 0.0f;
    Map* m_map = nullptr; // Pointer to the map for destruction
    void destroy_wall_in_direction(float direction);
    
    int hp;


    float m_width = 1.0f,
          m_height = 1.0f;
    // ————— COLLISIONS ————— //
    bool m_collided_top    = false;
    bool m_collided_bottom = false;
    bool m_collided_left   = false;
    bool m_collided_right  = false;

public:
    // ————— STATIC VARIABLES ————— //
    static constexpr int SECONDS_PER_FRAME = 4;

    // ————— METHODS ————— //
    Entity();
    Entity(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power, int walking[4][8], float animation_time,
        int animation_frames, int animation_index, int animation_cols,
           int animation_rows, float width, float height, EntityType EntityType);
    Entity(GLuint texture_id, float speed, float width, float height, EntityType EntityType); // Simpler constructor
    Entity(GLuint texture_id, float speed, float width, float height, EntityType EntityType, AIType AIType, AIState AIState); // AI constructor
    ~Entity();

    void draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index);
    bool const check_collision(Entity* other) const;
    
    void const check_collision_y(Entity* collidable_entities, int collidable_entity_count);
    void const check_collision_x(Entity* collidable_entities, int collidable_entity_count);
    
    // Overloading our methods to check for only the map
    void const check_collision_y(Map *map);
    void const check_collision_x(Map *map);
    
    void update(float delta_time, Entity *player, Entity *collidable_entities, int collidable_entity_count, Map *map);
    void render(ShaderProgram* program);
    
    void ai_activate(Entity *player);
    void ai_walk();
    void ai_crabominable(Entity *player);
    void ai_charger(Entity *player);
    void ai_charger2(Entity *player);
    void ai_boss(Entity *player);
    void set_map(Map* map) { m_map = map; }
    
    void normalise_movement() { m_movement = glm::normalize(m_movement); }

    void face_left() { m_animation_indices = m_walking[LEFT]; }
    void face_right() { m_animation_indices = m_walking[RIGHT]; }
    void face_up() { m_animation_indices = m_walking[UP]; }
    void face_down() { m_animation_indices = m_walking[DOWN]; }

    void move_left() { m_movement.x = -1.0f; face_left(); }
    void move_right() { m_movement.x = 1.0f;  face_right(); }
    void move_up() { m_movement.y = 1.0f;  face_up(); }
    void move_down() { m_movement.y = -1.0f; face_down(); }
    
    void const jump() { m_is_jumping = true; face_up();}

    // ————— GETTERS ————— //
    EntityType const get_entity_type()    const { return m_entity_type;   };
    AIType     const get_ai_type()        const { return m_ai_type;       };
    AIState    const get_ai_state()       const { return m_ai_state;      };
    float const get_jumping_power() const { return m_jumping_power; }
    glm::vec3 const get_position()     const { return m_position; }
    glm::vec3 const get_positionstart()     const { return m_start_pos; }
    glm::vec3 const get_velocity()     const { return m_velocity; }
    glm::vec3 const get_acceleration() const { return m_acceleration; }
    glm::vec3 const get_movement()     const { return m_movement; }
    glm::vec3 const get_scale()        const { return m_scale; }
    GLuint    const get_texture_id()   const { return m_texture_id; }
    float     const get_speed()        const { return m_speed; }
    bool      const get_collided_top() const { return m_collided_top; }
    bool      const get_collided_bottom() const { return m_collided_bottom; }
    bool      const get_collided_right() const { return m_collided_right; }
    bool      const get_collided_left() const { return m_collided_left; }
    bool      const get_next_scene() const {return is_next;}
    int const get_hp() const {return hp;}
    bool const get_gameover() const {return is_gameover;}
    bool const get_win() const {return is_win;}
    
    
    void activate()   { m_is_active = true;  };
    void deactivate() { m_is_active = false; };
    // ————— SETTERS ————— //
    void const set_entity_type(EntityType new_entity_type)  { m_entity_type = new_entity_type;};
    void const set_ai_type(AIType new_ai_type){ m_ai_type = new_ai_type;};
    void const set_ai_state(AIState new_state){ m_ai_state = new_state;};
    void const set_position(glm::vec3 new_position) { m_position = new_position; }
    void const set_positionstart(glm::vec3 new_position) { m_start_pos = new_position;}
    void const set_velocity(glm::vec3 new_velocity) { m_velocity = new_velocity; }
    void const set_acceleration(glm::vec3 new_acceleration) { m_acceleration = new_acceleration; }
    void const set_movement(glm::vec3 new_movement) { m_movement = new_movement; }
    void const set_scale(glm::vec3 new_scale) { m_scale = new_scale; }
    void const set_texture_id(GLuint new_texture_id) { m_texture_id = new_texture_id; }
    void const set_texture_win(GLuint new_texture_id) { m_win_texture = new_texture_id; }
    void const set_texture_lose(GLuint new_texture_id) { m_lose_texture = new_texture_id; }
    void const set_speed(float new_speed) { m_speed = new_speed; }
    void const set_animation_cols(int new_cols) { m_animation_cols = new_cols; }
    void const set_animation_rows(int new_rows) { m_animation_rows = new_rows; }
    void const set_animation_frames(int new_frames) { m_animation_frames = new_frames; }
    void const set_animation_index(int new_index) { m_animation_index = new_index; }
    void const set_animation_time(float new_time) { m_animation_time = new_time; }
    void const set_jumping_power(float new_jumping_power) { m_jumping_power = new_jumping_power;}
    void const set_width(float new_width) {m_width = new_width; }
    void const set_height(float new_height) {m_height = new_height; }
    void const set_hp(int new_hp) {hp = new_hp;}
    void const set_next_scene(bool is_new_scene) {is_next = is_new_scene;}
    void const die();
    void const set_gameover(){if(hp==0){hp ==0; is_gameover = true; set_texture_id(m_lose_texture);}}
    void const set_win(){is_win = true; set_texture_id(m_win_texture);}
    // Setter for m_walking
    void set_walking(int walking[4][8])
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                m_walking[i][j] = walking[i][j];
            }
        }
    }
    
    void set_charging(int charging[14][8])
    {
        for (int i = 0; i < 14; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                m_walking[i][j] = charging[i][j];
            }
        }

    }
};

#endif // ENTITY_H
