#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"

Mix_Chunk *hurt_sfx = Mix_LoadWAV("assets/lobotomy.mp3");;
Mix_Chunk *win_sfx;




void Entity::ai_activate(Entity *player)
{
    switch (m_ai_type)
    {
        case WALKER:
            ai_walk();
            break;
            
        case GUARD:
            ai_crabominable(player);
            break;
            
        case CHARGER:
            ai_charger(player);
            break;
            
            
        case CHARGER2:
            ai_charger2(player);
            break;
            
        case BIG_BOSS:
            ai_boss(player);
            break;
            
        default:
            break;
    }
}

void Entity::ai_walk()
{
    if (m_movement.x > 0.1f) face_right(); // Walk right animation
    else if (m_movement.x < -0.1f) face_left(); // Walk left animation
}
void Entity::ai_crabominable(Entity *player)
{
    if(check_collision(player) || get_collided_top())
        player->die();

    switch (m_ai_state) {
        case IDLE:
            if (glm::distance(m_position, player->get_position()) < 3.0f) m_ai_state = WALKING;
            break;
            
        case WALKING:
            if (m_position.x > player->get_position().x) {
                m_movement = glm::vec3(-3.0f, 0.0f, 0.0f);
                face_left();
            } else {
                m_movement = glm::vec3(3.0f, 0.0f, 0.0f);
                face_right();
            }
            break;
            
        case ATTACKING:
            break;
            
        default:
            break;
    }
}




void Entity::ai_charger(Entity* player)
{
    static glm::vec3 charge_start_position;
    static float charge_direction = 1.0f; // Default to right
    static bool has_collided = false;
    
    m_acceleration.y = 0;

    if(check_collision(player))
        player->die();
    
    switch (m_ai_state) {
        case IDLE:
            if (glm::distance(m_position.x, player->get_position().x) < 4.0f &&
                glm::distance(m_position.y, player->get_position().y) < 1.0f) {
                charge_start_position = m_position;
                charge_direction = (m_position.x > player->get_position().x) ? -1.0f : 1.0f;
                m_ai_state = ATTACKING;
              
            }
            break;

        case ATTACKING:
            // Move in committed direction
            m_movement.x = 8.0f * charge_direction;
            (charge_direction < 0) ? face_left() : face_right();
            
            
            // Transition conditions
            if (glm::distance(m_position, charge_start_position) > 9.0f) {
                m_ai_state = WALKING;
            }
            break;
            
        case WALKING:
            // Return to starting position
            float return_direction = (m_position.x > charge_start_position.x) ? -1.0f : 1.0f;
            m_movement.x = 0.5f * return_direction;
            (return_direction < 0) ? face_left() : face_right();
            
            // Return to idle when close to start position
            if (glm::distance(m_position, charge_start_position) < 0.5f) {
                m_movement = glm::vec3(0.0f);
                m_ai_state = IDLE;
            }
            break;
    }
}


//will charge at player regardless of vertical distance, used for walls
void Entity::ai_charger2(Entity* player)
{
    static glm::vec3 charge_start_position;
    static float charge_direction = 1.0f; // Default to right
    
    m_acceleration.y = 0;

    if(check_collision(player))
        player->die();
    
    switch (m_ai_state) {
        case IDLE:
            if (glm::distance(m_position.x, player->get_position().x) < 5.0f)
            {
                charge_start_position = m_position;
                charge_direction = (m_position.x > player->get_position().x) ? -1.0f : 1.0f;
                m_ai_state = ATTACKING;
            }
            break;

        case ATTACKING:
            // Move in committed direction
            m_movement.x = 6.0f * charge_direction;
            (charge_direction < 0) ? face_left() : face_right();
             
            // Transition conditions
            if (glm::distance(m_position, charge_start_position) > 10.0f) {
                m_ai_state = WALKING;
            }
            break;
            
        case WALKING:
            // Return to starting position
            float return_direction = (m_position.x > charge_start_position.x) ? -1.0f : 1.0f;
            m_movement.x = 1.0f * return_direction;
            (return_direction < 0) ? face_left() : face_right();
            
            // Return to idle when close to start position
            if (glm::distance(m_position, charge_start_position) < 0.5f) {
                m_movement = glm::vec3(0.0f);
                m_ai_state = IDLE;
            }
            break;
    }
}


void Entity::ai_boss(Entity* player)
{
    static glm::vec3 charge_start_position;
    static float charge_direction = 1.0f; // Default to right
    static bool has_collided = false;
    
    float return_direction = (m_position.x > charge_start_position.x) ? -1.0f : 1.0f;

    m_acceleration.y = 0;
    if(check_collision(player))
        player->die();
    
    switch (m_ai_state) {
            
            
        case WALKING4:
            // Calculate return direction (opposite of WALKING behavior)
               
               // Move toward starting position
               m_movement.x = 1.0f * return_direction;
               (return_direction < 0) ? face_left() : face_right();
               
               // Return to IDLE2 when TO THE LEFT OF PLAYER ONLY
               if (glm::distance(m_position.x, player->get_position().x) > 3.0f &&
                   m_position.x < player->get_position().x)
               {
                   m_movement = glm::vec3(0.0f);
                   m_ai_state = IDLE3;
                   
                 
               }
               break;
            
        case ATTACKING2:
            // Move in committed direction
            m_movement.x = 7.5f * charge_direction;
            (charge_direction < 0) ? face_left() : face_right();
            
            // Handle wall collision and destruction
            if ((charge_direction < 0 && m_collided_left) ||
                (charge_direction > 0 && m_collided_right)) {
                if (!has_collided) {
                    destroy_wall_in_direction(charge_direction);
                    has_collided = true;
                    // Reset collision flags to continue movement
                    m_collided_left = false;
                    m_collided_right = false;
                }
            }
            
            // Transition conditions
            if (glm::distance(m_position, charge_start_position) > 11.0f) {
                m_ai_state = WALKING4;
            }
            break;
        case IDLE3:
            if (glm::distance(m_position.x, player->get_position().x) < 6.0f &&
                glm::distance(m_position.y, player->get_position().y) < 1.0f) {
                charge_start_position = m_position;
                charge_direction = (m_position.x > player->get_position().x) ? -1.0f : 1.0f;
                m_ai_state = ATTACKING2;
                has_collided = false;
            }
            
            break;
        
        case WALKING3:
                    // Move upward
            m_velocity.y = 2.0f;
            m_movement.y= 1.0f;
                    
                    face_down();
                    
                    if (m_position.y >= -1.2f) {
                        m_movement = glm::vec3(0.0f);
                        m_velocity.y = 0.0f;
                        m_ai_state = IDLE3;
                        face_right();
                    }
                    break;
            
        case WALKING2:
            // walk left
            m_movement = glm::vec3(-3.0f, 0.0f, 0.0f);  // Move left
            face_left();
            
            // Check if reached left edge (x position <= some threshold)
            if (m_position.x <= 0.0) {  // Adjust this value to your map's left edge
                m_movement = glm::vec3(0.0f);
                m_ai_state = WALKING3;  // Transition to idle state
            }
            break;

        case IDLE1:
            if (glm::distance(m_position.x, player->get_position().x) < 6.0f &&
                glm::distance(m_position.y, player->get_position().y) < 1.0f) {
                charge_start_position = m_position;
                charge_direction = (m_position.x > player->get_position().x) ? -1.0f : 1.0f;
                m_ai_state = ATTACKING1;
                has_collided = false;
            }
            
            break;
        case ATTACKING1:
            // Move in committed direction. relative to players speed
        
            m_movement.x = 5.0f * charge_direction;
            (charge_direction < 0) ? face_left() : face_right();
            
            // Handle wall collision and destruction
            if ((charge_direction < 0 && m_collided_left) ||
                (charge_direction > 0 && m_collided_right)) {
                if (!has_collided) {
                    destroy_wall_in_direction(charge_direction);
                    has_collided = true;
                    // Reset collision flags to continue movement
                    m_collided_left = false;
                    m_collided_right = false;
                }
            }
            
            // Transition conditions
            if (glm::distance(m_position, charge_start_position) > 15.0f) {
                m_ai_state = WALKING2;
            }
            break;
        case WALKING1:
            if (m_position.x > player->get_position().x) {
                m_movement = glm::vec3(-5.0f, 0.0f, 0.0f);
                face_left();
            } else {
                m_movement = glm::vec3(5.0f, 0.0f, 0.0f);
                face_right();
            }
            
            if (abs(m_position.x - player->get_position().x) < 1.0f &&  // X position aligned (with tolerance)
                   player->get_position().y > m_position.y) {              // Player is firectly above
                   m_movement = glm::vec3(0.0f);
                   m_ai_state = IDLE1;
               }
               break;
            
        case IDLE2:
            if (glm::distance(m_position, player->get_position()) < 4.0f) m_ai_state = WALKING1;
            break;
            
        case IDLE:
            if (glm::distance(m_position.x, player->get_position().x) < 6.00000000000000005f &&
                glm::distance(m_position.y, player->get_position().y) < 1.0f) {
                charge_start_position = m_position;
                charge_direction = (m_position.x > player->get_position().x) ? -1.0f : 1.0f;
                m_ai_state = ATTACKING;
                has_collided = false;
            }
            break;

        case ATTACKING:
            // Move in committed direction
            m_movement.x = 7.5f * charge_direction;
            (charge_direction < 0) ? face_left() : face_right();
            
            // Handle wall collision and destruction
            if ((charge_direction < 0 && m_collided_left) ||
                (charge_direction > 0 && m_collided_right)) {
                if (!has_collided) {
                    destroy_wall_in_direction(charge_direction);
                    has_collided = true;
                    // Reset collision flags to continue movement
                    m_collided_left = false;
                    m_collided_right = false;
                }
            }
            
            // Transition conditions
            if (glm::distance(m_position, charge_start_position) > 9.0f) {
                m_ai_state = WALKING;
            }
            break;
            
        case WALKING:
            // Return to starting position
          
            m_movement.x = 1.0f * return_direction;
            (return_direction < 0) ? face_left() : face_right();
            
            // Return to idle2 when TO THE RIGHT OF PLAYER ONLY
            if (glm::distance(m_position.x, player->get_position().x) > 4.0f && m_position.x>player->get_position().x) {
                m_movement = glm::vec3(0.0f);
                face_left();
                m_ai_state = IDLE2;
            }
    
            break;
       
            
    }
}




void Entity::destroy_wall_in_direction(float direction)
{
    if (!m_map) return;
    
    // Calculate wall position (adjust offset as needed)
    float offset = (direction < 0) ? -m_width : m_width;
    glm::vec3 wall_pos = glm::vec3(
        m_position.x + offset,
        m_position.y,
        m_position.z
    );
    
    // Destroy the wall tile
    m_map->destroy_tile_at(wall_pos);
    
    // Optional: Add visual/audio effects
    // play_sound("wall_break.wav");
    // spawn_particles(wall_pos);
}


// Default constructor
Entity::Entity()
    : m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
    m_speed(0.0f), m_animation_cols(0), m_animation_frames(0), m_animation_index(0),
    m_animation_rows(0), m_animation_indices(nullptr), m_animation_time(0.0f),
    m_texture_id(0), m_velocity(0.0f), m_acceleration(0.0f), m_width(0.0f), m_height(0.0f)
{
    // Initialize m_walking with zeros or any default value
    for (int i = 0; i < SECONDS_PER_FRAME; ++i)
        for (int j = 0; j < SECONDS_PER_FRAME; ++j) m_walking[i][j] = 0;
}

// Parameterized constructor
Entity::Entity(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power, int walking[4][8], float animation_time,
    int animation_frames, int animation_index, int animation_cols,
    int animation_rows, float width, float height, EntityType EntityType)
    : m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
    m_speed(speed),m_acceleration(acceleration), m_jumping_power(jump_power), m_animation_cols(animation_cols),
    m_animation_frames(animation_frames), m_animation_index(animation_index),
    m_animation_rows(animation_rows), m_animation_indices(nullptr),
    m_animation_time(animation_time), m_texture_id(texture_id), m_velocity(0.0f),
    m_width(width), m_height(height), m_entity_type(EntityType)
{
    face_right();
    set_walking(walking);
    
}

// Simpler constructor for partial initialization
Entity::Entity(GLuint texture_id, float speed,  float width, float height, EntityType EntityType)
    : m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
    m_speed(speed), m_animation_cols(0), m_animation_frames(0), m_animation_index(0),
    m_animation_rows(0), m_animation_indices(nullptr), m_animation_time(0.0f),
    m_texture_id(texture_id), m_velocity(0.0f), m_acceleration(0.0f), m_width(width), m_height(height),m_entity_type(EntityType)
{
    // Initialize m_walking with zeros or any default value
    for (int i = 0; i < SECONDS_PER_FRAME; ++i)
        for (int j = 0; j < SECONDS_PER_FRAME; ++j) m_walking[i][j] = 0;
}
Entity::Entity(GLuint texture_id, float speed, float width, float height, EntityType EntityType, AIType AIType, AIState AIState): m_position(0.0f), m_movement(0.0f), m_scale(1.0f, 1.0f, 0.0f), m_model_matrix(1.0f),
m_speed(speed), m_animation_cols(0), m_animation_frames(0), m_animation_index(0),
m_animation_rows(0), m_animation_indices(nullptr), m_animation_time(0.0f),
m_texture_id(texture_id), m_velocity(0.0f), m_acceleration(0.0f), m_width(width), m_height(height),m_entity_type(EntityType), m_ai_type(AIType), m_ai_state(AIState)
{
// Initialize m_walking with zeros or any default value
for (int i = 0; i < SECONDS_PER_FRAME; ++i)
    for (int j = 0; j < SECONDS_PER_FRAME; ++j) m_walking[i][j] = 0;
}

Entity::~Entity() { }

void Entity::draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index)
{
    // Step 1: Calculate the UV location of the indexed frame
    
    float u_coord = (float)(index % m_animation_cols) / (float)m_animation_cols;
    float v_coord = (float)(index / m_animation_cols) / (float)m_animation_rows;

    // Step 2: Calculate its UV size
    float width = 1.0f / (float)m_animation_cols;
    float height = 1.0f / (float)m_animation_rows;

    // Step 3: Just as we have done before, match the texture coordinates to the vertices
    float tex_coords[] =
    {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };

    float vertices[] =
    {
        -0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
        -0.5, -0.5, 0.5,  0.5, -0.5, 0.5
    };

    // Step 4: And render
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());

    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

bool const Entity::check_collision(Entity* other) const
{
    float x_distance = fabs(m_position.x - other->m_position.x) - ((m_width + other->m_width) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((m_height + other->m_height) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
    
    
}

void const Entity::check_collision_y(Entity *collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity *collidable_entity = &collidable_entities[i];
        
        if (check_collision(collidable_entity))
        {
            float y_distance = fabs(m_position.y - collidable_entity->m_position.y);
            float y_overlap = fabs(y_distance - (m_height / 2.0f) - (collidable_entity->m_height / 2.0f));
            if (m_velocity.y > 0)
            {
                m_position.y   -= y_overlap;
                m_velocity.y    = 0;

                // Collision!
                m_collided_top  = true;
            } else if (m_velocity.y < 0)
            {
                m_position.y      += y_overlap;
                m_velocity.y       = 0;

                // Collision!
                m_collided_bottom  = true;
            }
        }
    }
}

void const Entity::check_collision_x(Entity *collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity *collidable_entity = &collidable_entities[i];
        
        if (check_collision(collidable_entity))
        {
            float x_distance = fabs(m_position.x - collidable_entity->m_position.x);
            float x_overlap = fabs(x_distance - (m_width / 2.0f) - (collidable_entity->m_width / 2.0f));
            if (m_velocity.x > 0)
            {
                m_position.x     -= x_overlap;
                m_velocity.x      = 0;

                // Collision!
                m_collided_right  = true;
                
            } else if (m_velocity.x < 0)
            {
                m_position.x    += x_overlap;
                m_velocity.x     = 0;
 
                // Collision!
                m_collided_left  = true;
            }
        }
    }
}

void const Entity::check_collision_y(Map *map)
{
    // Probes for tiles above
    glm::vec3 top = glm::vec3(m_position.x, m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_left = glm::vec3(m_position.x - (m_width / 2), m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_right = glm::vec3(m_position.x + (m_width / 2), m_position.y + (m_height / 2), m_position.z);
    
    // Probes for tiles below
    glm::vec3 bottom = glm::vec3(m_position.x, m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_left = glm::vec3(m_position.x - (m_width / 2), m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_right = glm::vec3(m_position.x + (m_width / 2), m_position.y - (m_height / 2), m_position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    // If the map is solid, check the top three points
    if (map->is_solid(top, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }
    else if (map->is_solid(top_left, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }
    else if (map->is_solid(top_right, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }
    
    // And the bottom three points
    if (map->is_solid(bottom, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
    }
    else if (map->is_solid(bottom_left, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
            m_position.y += penetration_y;
            m_velocity.y = 0;
            m_collided_bottom = true;
    }
    else if (map->is_solid(bottom_right, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
        
    }
}

void const Entity::check_collision_x(Map *map)
{
    // Probes for tiles; the x-checking is much simpler
    glm::vec3 left  = glm::vec3(m_position.x - (m_width / 2), m_position.y, m_position.z);
    glm::vec3 right = glm::vec3(m_position.x + (m_width / 2), m_position.y, m_position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->is_solid(left, &penetration_x, &penetration_y) && m_velocity.x < 0)
    {
        m_position.x += penetration_x;
        m_velocity.x = 0;
        m_collided_left = true;
    }
    if (map->is_solid(right, &penetration_x, &penetration_y) && m_velocity.x > 0)
    {
        m_position.x -= penetration_x;
        m_velocity.x = 0;
        m_collided_right = true;
    }
}

void const Entity::die()
{
    hp--;
    set_position(m_start_pos);
    Mix_PlayChannel(-1, hurt_sfx, 0);
    
}
void Entity::update(float delta_time, Entity *player, Entity *collidable_entities, int collidable_entity_count, Map *map)
{
    if (!m_is_active) return;
 
    
    
    if(m_entity_type == PLAYER && check_collision(collidable_entities))
    {
        die();
        printf("died");
        printf("%d",hp);
        if(hp == 0)
        {
            set_gameover();
        }
        if(is_gameover)
        {
            printf("cooked");
        }
    }
    
    
    m_collided_top    = false;
    m_collided_bottom = false;
    m_collided_left   = false;
    m_collided_right  = false;
    
    if (m_entity_type == ENEMY) ai_activate(player);
    
    if (m_entity_type == ENEMY && player != nullptr)
    {
        if(check_collision(player))
        {
            if(player->hp>0)
            {
                player->hp--;
                
            }
        }
        
        if(player->hp <=0)
        {
            player->m_is_active == false;
        }
    }
   


    if (m_animation_indices != NULL)
    {
        if (glm::length(m_movement) != 0)
        {
            m_animation_time += delta_time;
            float frames_per_second = (float) 1 / SECONDS_PER_FRAME;
            
            if (m_animation_time >= frames_per_second)
            {
                m_animation_time = 0.0f;
                m_animation_index++;
                
                if (m_animation_index >= m_animation_frames)
                {
                    m_animation_index = 0;
                }
            }
        }
    }
    
    m_velocity.x = m_movement.x * m_speed;
    m_velocity += m_acceleration * delta_time;
    
    if (m_is_jumping)
    {
        m_is_jumping = false;
        m_velocity.y += m_jumping_power;
    }
    
    m_position.y += m_velocity.y * delta_time;
    
    check_collision_y(collidable_entities, collidable_entity_count);
    check_collision_y(map);
    
    m_position.x += m_velocity.x * delta_time;
    check_collision_x(collidable_entities, collidable_entity_count);
    check_collision_x(map);
    
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
}


void Entity::render(ShaderProgram* program)
{
    
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
    m_model_matrix = glm::scale(m_model_matrix, m_scale);
    program->set_model_matrix(m_model_matrix);

    if (m_animation_indices != NULL)
    {
        draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}
