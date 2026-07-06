#pragma once 

#include "engine/world.hpp"
#include "engine/scheduler.hpp"

class Engine {

private:
    void registerComponents(World& world);

public:
    void startUp(World &world);
    void run(World &world);
    void shutDown(World &world);

    Scheduler scheduler;
};
