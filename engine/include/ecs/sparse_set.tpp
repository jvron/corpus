#pragma once

#include "ecs/sparse_set.hpp"
#include "ecs/components.hpp"
#include <cstddef>

template<typename Component> 
void SparseSet<Component>::insert(const Component &component,const Entity &entity) {

    const size_t denseArraySize = denseArray.size();

    denseArray.emplace_back(component);
    entityArray.emplace_back(entity);

    sparseArray[entity.id] = denseArraySize;
}

template <typename Component>
Component* SparseSet<Component>::lookup(const Entity &entity) {

    int index = sparseArray[entity.id];

    if (index >= denseArray.size() || index < 0) {
        return nullptr;
    }
    return &denseArray[index]; 
    
}

template<typename Component>
void SparseSet<Component>::remove(const Entity &entity) {

    int denseIndex = sparseArray[entity.id]; 
    
    if (denseIndex >= denseArray.size() || denseIndex < 0) {
        return;
    }

    const size_t denseArraySize = denseArray.size();
    const size_t entityArraySize = entityArray.size();

    if (denseArraySize > 1 && denseIndex != (denseArraySize - 1)) {

        // sparseArray[entity] = index of dense array of that entity
        Entity lastEntity = entityArray[entityArraySize - 1];
        
        denseArray[denseIndex] = denseArray[denseArraySize - 1];
        entityArray[denseIndex] = entityArray[entityArraySize - 1];

        //update moved entity to its new index
        sparseArray[lastEntity.id] = denseIndex;
    }
    denseArray.pop_back();
    entityArray.pop_back();
    sparseArray[entity.id] = -1; // mark as invalid, will be reused
}
