#pragma once 
#include "engine/registry.hpp"
#include <unordered_map>

using systemFn = void (*)(Registry &registry);

// schedule is a group of systems to be run together
struct Schedule {
    std::vector<systemFn> systems; 
};
enum class Stage{
    Window,
    Input, 
    Render,
    Update
};

class Scheduler {

    public:
    void init(Registry &registry);
    void runStage(Stage stage);
    void addSystem(Stage stage, systemFn sys);
    std::unordered_map<Stage, Schedule> pipeline;
    Registry *registryHandle;
};