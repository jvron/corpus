#pragma once 

#include <cstddef>
#include <vector>

#include "engine/world.hpp"

constexpr size_t StageCount = 7;

using SystemFn = void (*)(World& world);

// schedule is a group of systems to be run together
struct Schedule {
    std::vector<SystemFn> systems {}; 
};

enum class Stage{
    Startup,
    Begin,
    Input, 
    Update,
    Render,
    End,
    Shutdown
};

class Scheduler {

public:
    void init(World& world);
    void runStage(Stage stage);
    void addSystem(Stage stage, SystemFn sys);

private:
    World *worldHandle = nullptr;
    std::array<Schedule, StageCount> pipeline;
};