#include <cassert>
#include <cstddef>
#include <vector>

#include "corpus/scheduler.hpp"
#include "corpus/world.hpp"

void Scheduler::init(World& world) {
    worldHandle = &world;
}

void Scheduler::runStage(Stage stage) {
    
    const Schedule& shedule = pipeline[static_cast<size_t>(stage)];
    for (auto sys : shedule.systems) {
        sys(*worldHandle);
    }
}

void Scheduler::addSystem(Stage stage, SystemFn sys) {

    pipeline[static_cast<size_t>(stage)].systems.push_back(sys);
}