#pragma once

class Settings {
    public:
        int swarm_size = 400;

        float protected_range = 20.0f;
        float visual_range = 80.0f;
        float predator_range = 120.0f;

        float matching_factor = 0.05f;  
        float centering_factor = 0.005f; 
        float avoid_factor = 0.2f;
        float flee_factor = 1.5f;
        
        float prey_min_speed = 2.0f;
        float prey_max_speed = 5.0f;
        float predator_min_speed = 2.0f;
        float predator_max_speed = 7.0f;
};



