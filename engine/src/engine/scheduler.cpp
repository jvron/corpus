#include <cstddef>
#include <vector>

#include "engine/scheduler.hpp"
#include "engine/world.hpp"

void Scheduler::init(World &world) {
    worldHandle = &world;
}
 
void Scheduler::runStage(Stage stage) {
    
    const Schedule& shedule = pipeline[static_cast<size_t>(stage)];
    for (auto sys : shedule.systems) {
        sys(*worldHandle);
    }
}

void Scheduler::addSystem(Stage stage, systemFn sys) {

    pipeline[static_cast<size_t>(stage)].systems.push_back(sys);
}