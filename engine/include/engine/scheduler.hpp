#pragma once 

#include <unordered_map>
#include <vector>

#include "engine/world.hpp"

using systemFn = void (*)(World &world);

// schedule is a group of systems to be run together
struct Schedule {
    std::vector<systemFn> systems {}; 
};

enum class Stage{
    Begin,
    Input, 
    Update,
    Render,
    End
};

class Scheduler {

public:
    void init(World &world);
    void runStage(Stage stage);
    void addSystem(Stage stage, systemFn sys);

private:
    World *worldHandle = nullptr;
    std::unordered_map<Stage, Schedule> pipeline;
};