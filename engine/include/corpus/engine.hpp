#pragma once 

#include "world.hpp"
#include "scheduler.hpp"

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
