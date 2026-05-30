// external engine headers (to be used by game/editor)

#pragma once 
#include "engine/registry.hpp"
#include "engine/scheduler.hpp"


namespace Engine {

    inline Scheduler scheduler;
    void startUp(Registry &registry);
    void run(Registry &registry);
    void shutDown(Registry &registry);
};
