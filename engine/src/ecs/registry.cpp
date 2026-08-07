#include <cstddef>
#include <cstdint>

#include "ecs/registry.hpp"
#include "ecs/components.hpp"

bool Registry::isValidEntity(Entity entity) {

    if (entity.id >= generations.size() || entity.generation != generations[entity.id]) {
        return false;
    }
    return true;
}

Entity Registry::createEntity() {

    Entity entity;

    if (!freeEntityIDs.empty()) {
        EntityID id = freeEntityIDs.back();

        if (id < generations.size()) {
            entity.id = id;
            entity.generation = generations[id];
            freeEntityIDs.pop_back(); // id is no longer free as it is used by the new entity 
        }
    }
    else {
        uint32_t generation = 0;
        EntityID id = generations.size();
        generations.push_back(generation);

        entity.id = id;
        entity.generation = generation;
    }

    size_t index = activeEntities.size();

    if (entity.id >= entityLocations.size()) {
        entityLocations.push_back(index); // new id
    }
    else {
        entityLocations[entity.id] = index; // reuse id
    }

    activeEntities.push_back(entity);

    return entity;
}

void Registry::destroyEntity(Entity entity) {

    if (!isValidEntity(entity)) {
        return;
    }

    freeEntityIDs.push_back(entity.id);
    generations[entity.id]++;

    for (auto& pool : pools) {
        if (pool.hasEntity(entity)) {
            pool.remove(entity);
        }
    }

    size_t index = entityLocations[entity.id];

    Entity lastEntity = activeEntities.back();
    activeEntities[index] = lastEntity;
    activeEntities.pop_back();

    entityLocations[lastEntity.id] = index;
}

std::span<const Entity> Registry::entities() const {
    return activeEntities;
}

size_t Registry::entityCount() const {
    return activeEntities.size();
}