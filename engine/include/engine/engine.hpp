#pragma once 

#include "engine/world.hpp"
#include "engine/scheduler.hpp"

class Engine {

public:
    void startup(World& world);
    void runFrame(World& world);
    void run(World& world);
    void shutdown(World& world);

    Scheduler scheduler;

private:
    void registerComponents(World& world);
};
