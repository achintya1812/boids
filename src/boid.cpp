#include "boid.hpp"
#include "settings.hpp"

#include "raymath.h"
#include <cmath>

Boid::Boid(float x, float y) {
    position = {x, y};
    
    velocity = { (float)GetRandomValue(-10, 10), (float)GetRandomValue(-10, 10)};
    velocity = Vector2Normalize(velocity);

    unsigned char r = (unsigned char)GetRandomValue(100, 255);
    unsigned char g = (unsigned char)GetRandomValue(100, 255);
    unsigned char b = (unsigned char)GetRandomValue(100, 255);
    
    color = {r, g, b, 255};
}

void Boid::update(const std::vector<Boid>& flock, Vector2 predator_position, const Settings& settings) {
    Vector2 v1 = separation(flock, settings.protected_range);
    Vector2 v2 = alignment(flock, settings.visual_range);
    Vector2 v3 = cohesion(flock, settings.visual_range);
    Vector2 v4 = flee(predator_position, settings.predator_range);

    v1 = Vector2Scale(v1, settings.avoid_factor);
    v2 = Vector2Scale(v2, settings.matching_factor);
    v3 = Vector2Scale(v3, settings.centering_factor);
    v4 = Vector2Scale(v4, settings.flee_factor);

    velocity = Vector2Add(velocity, v1);
    velocity = Vector2Add(velocity, v2);
    velocity = Vector2Add(velocity, v3);
    velocity = Vector2Add(velocity, v4);

    float margin = 50.0f;
    float turn_force = 1.0f;
    if (position.x < margin) velocity.x += turn_force;
    if (position.x > GetScreenWidth() - margin) velocity.x -= turn_force;
    if (position.y < margin) velocity.y += turn_force;
    if (position.y > GetScreenHeight() - margin) velocity.y -= turn_force;

    float speed = Vector2Length(velocity);
    
    if (speed > 0.0f) {
        if (speed > settings.prey_max_speed) {
            velocity.x = (velocity.x / speed) * settings.prey_max_speed;
            velocity.y = (velocity.y / speed) * settings.prey_max_speed;
        } else if (speed < settings.prey_min_speed) {
            velocity.x = (velocity.x / speed) * settings.prey_min_speed;
            velocity.y = (velocity.y / speed) * settings.prey_min_speed;
        }
    }
    
    position = Vector2Add(position, velocity);
}

void Boid::draw() {
    const float side_length = 25.0f;

    // distance from centre to vertex 
    const float r = side_length / std::sqrt(3.0f);

    float theta = std::atan2f(velocity.y, velocity.x);
    
    Vector2 vertex1 = {position.x + r * std::cosf(theta), position.y + r * std::sinf(theta)};
    Vector2 vertex2 = {position.x + r * std::cosf(theta + 2.0944f), position.y + r * std::sinf(theta + 2.0944f)};
    Vector2 vertex3 = {position.x + r * std::cosf(theta + 4.1888f), position.y + r * std::sinf(theta + 4.1888f)};

    DrawTriangle(vertex1, vertex3, vertex2, color);
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

Vector2 Boid::flee(const Vector2& predator_position, float predator_range) {
    float distance = Vector2Distance(position, predator_position);

    if (distance >= predator_range) {
        return {0.0f, 0.0f};
    }

    if (distance == 0.0f) {
        distance = 0.001f;
    }

    Vector2 away = Vector2Subtract(position, predator_position);
    away = Vector2Normalize(away);

    float strength = (predator_range - distance) / predator_range;
    return Vector2Scale(away, strength);
}
