#include "boid.hpp"
#include "raymath.h"

Boid::Boid(float x, float y) {
    position = {x, y};
    
    velocity = { (float)GetRandomValue(-10, 10), (float)GetRandomValue(-10, 10)};
    velocity = Vector2Normalize(velocity);
}

void Boid::update(const std::vector<Boid>& flock) {
    float protected_range = 20.0f;
    float visual_range = 80.0f;

    float matching_factor = 0.05f;  
    float centering_factor = 0.005f; 
    float avoid_factor = 0.2f;

    Vector2 v1 = separation(flock, protected_range);
    Vector2 v2 = alignment(flock, visual_range);
    Vector2 v3 = cohesion(flock, visual_range);

    // CORRECTED SCALING
    v1 = Vector2Scale(v1, avoid_factor);
    v2 = Vector2Scale(v2, matching_factor);
    v3 = Vector2Scale(v3, centering_factor);

    velocity = Vector2Add(velocity, v1);
    velocity = Vector2Add(velocity, v2);
    velocity = Vector2Add(velocity, v3);

    float margin = 50.0f;
    float turnForce = 1.0f;
    if (position.x < margin) velocity.x += turnForce;
    if (position.x > GetScreenWidth() - margin) velocity.x -= turnForce;
    if (position.y < margin) velocity.y += turnForce;
    if (position.y > GetScreenHeight() - margin) velocity.y -= turnForce;

    // ADDED MIN SPEED SO THEY DON'T STALL
    float speed = Vector2Length(velocity);
    float max_speed = 5.0f;
    float min_speed = 2.0f; 
    
    if (speed > 0.0f) {
        if (speed > max_speed) {
            velocity.x = (velocity.x / speed) * max_speed;
            velocity.y = (velocity.y / speed) * max_speed;
        } else if (speed < min_speed) {
            velocity.x = (velocity.x / speed) * min_speed;
            velocity.y = (velocity.y / speed) * min_speed;
        }
    }
    
    position = Vector2Add(position, velocity);
}

void Boid::draw() {
    DrawCircleV(position, 5.0f, DARKBLUE);
}   

Vector2 Boid::separation(const std::vector<Boid>& flock, float protected_range) {
    Vector2 c = {0.0f, 0.0f};

    for (int i = 0; i < flock.size(); i++) {
        if (&flock[i] != this) {
            float distance = Vector2Distance(position, flock[i].position);

            if (distance == 0.0f) {
                distance = 0.001f;
                position.x += 0.01f;
            }

            if (distance < protected_range) {
                Vector2 push = Vector2Subtract(position, flock[i].position);
                push = Vector2Normalize(push);

                float strength = (protected_range - distance) / protected_range;
                push = Vector2Scale(push, strength);

                c = Vector2Add(c, push);
            }
        }
    }

    return c;
}

Vector2 Boid::alignment(const std::vector<Boid>& flock, float visual_range) {
    Vector2 percieved_velocity = {0.0f, 0.0f};
    int neighbour_count = 0;

    for (int i = 0; i < flock.size(); i++) {
        if (&flock[i] != this) {
            float distance = Vector2Distance(position, flock[i].position);

            if (distance < visual_range) {
                percieved_velocity = Vector2Add(percieved_velocity, flock[i].velocity);
                neighbour_count++;
            }
        }
    }

    if (neighbour_count > 0) {
        percieved_velocity.x /= neighbour_count;
        percieved_velocity.y /= neighbour_count;

        return Vector2Subtract(percieved_velocity, velocity);
    }

    return {0.0f, 0.0f};

}

Vector2 Boid::cohesion(const std::vector<Boid>& flock, float visual_range) {
    Vector2 percieved_center = {0.0f, 0.0f};
    int neighbour_count = 0;

    for (int i = 0; i < flock.size(); i++) {
        if (&flock[i] != this) {
            float distance = Vector2Distance(position, flock[i].position);

            if (distance < visual_range) {
                percieved_center = Vector2Add(percieved_center, flock[i].position);
                neighbour_count++;
            }
        }
    }

    if (neighbour_count > 0) {
        percieved_center.x /= neighbour_count;
        percieved_center.y /= neighbour_count;
        
        return Vector2Subtract(percieved_center, position);
    }

    return {0.0f, 0.0f};
}