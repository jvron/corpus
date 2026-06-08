#include "ecs/sparse_set.hpp"
#include "ecs/components.hpp"
#include <cstddef>
#include <cstring>

const size_t DynamicSparseSet::size() const {
    return denseEntities.size();
}

bool DynamicSparseSet::hasEntity(const Entity &entity) const {
    int denseIndex = sparse[entity.id];

        if (denseIndex >= denseEntities.size() || denseIndex < 0) {
            return false;
        }

        if (denseEntities[denseIndex].id == entity.id) {
            return true;
        }
        else {
            return false;
        }
    }

void DynamicSparseSet::write(const Entity &entity, const void* componentSourceBytes) {
    if (hasEntity(entity)) {
        return;
    }
    //will be inserted at the end of the dense arrays
    int denseIndex = denseEntities.size();
    denseEntities.push_back(entity);
    sparse[entity.id] = denseIndex;

    size_t oldSize = denseComponents.size();
    denseComponents.resize(oldSize + componentSize);
    
    std::byte* destination = denseComponents.data() + (denseIndex * componentSize);

    std::memcpy(destination, componentSourceBytes, componentSize);
}

void DynamicSparseSet::remove(const Entity &entity) {

    if (!hasEntity(entity)) {
        return;
    }
    int denseIndex = sparse[entity.id];

    size_t denseSize = denseEntities.size();
    int lastDenseIndex = denseSize - 1;

    if (denseSize > 1 && denseIndex != (denseSize - 1)) {

        denseEntities[denseIndex] = denseEntities[denseSize - 1]; // copy last element

        Entity movedEntity = denseEntities[denseIndex];
        sparse[movedEntity.id] = denseIndex;


        std::byte* moveDestination = denseComponents.data() + (denseIndex * componentSize);
        //size of denseEntities index tells us how many components are stored 
        std::byte* lastComponent = denseComponents.data() + (lastDenseIndex * componentSize);  
    
        std::memcpy(moveDestination, lastComponent, componentSize);
    }
    denseEntities.pop_back();

    sparse[entity.id] = -1; //mark as deleted

    // shrink the array
    denseComponents.resize(denseComponents.size() - componentSize);
}

void* DynamicSparseSet::getRaw(const Entity &entity) {
    
    if (!hasEntity(entity)) {
        return nullptr;
    }
    int denseIndex = sparse[entity.id];

    std::byte* component = denseComponents.data() + (denseIndex * componentSize);

    return static_cast<void*>(component);
}