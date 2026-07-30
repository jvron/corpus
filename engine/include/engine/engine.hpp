#pragma once 

#include "engine/world.hpp"
#include "engine/scheduler.hpp"

class Engine {

public:
    void startUp(World &world);
    void run(World &world);
    void shutDown(World &world);

    Scheduler scheduler;

private:
    void registerComponents(World& world);
};
