#include <vector>
#include "engine/registry.hpp"
#include "engine/scheduler.hpp"


void Scheduler::init(Registry &registry) {
    registryHandle = &registry;

}
void Scheduler::runStage(Stage stage) {

    
    for (auto sys : pipeline.at(stage).systems) {
        sys(*registryHandle);
    }
}

void Scheduler::addSystem(Stage stage, systemFn sys) {
    pipeline[stage].systems.push_back(sys);
}