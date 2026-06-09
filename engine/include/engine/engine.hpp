#pragma once 

#include "engine/world.hpp"
#include "engine/scheduler.hpp"
#include <glad/glad.h>

class Engine {
    
public:
    void startUp(World &world);
    void run(World &world);
    void shutDown(World &world);

    Scheduler scheduler;
    //Renderer renderer;
};
