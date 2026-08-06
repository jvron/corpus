#include <vector>

#include "engine/scheduler.hpp"
#include "engine/world.hpp"

void Scheduler::init(World &world) {
    worldHandle = &world;
}
 
void Scheduler::runStage(Stage stage) {
    
    for (auto sys : pipeline.at(stage).systems) {
        sys(*worldHandle);
    }
}

void Scheduler::addSystem(Stage stage, systemFn sys) {
    pipeline[stage].systems.push_back(sys);
}