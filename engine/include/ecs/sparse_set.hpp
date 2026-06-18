#pragma once

#include <cstddef>
#include <vector>

#include "ecs/components.hpp"

constexpr int maxEntities = 100;
//constexpr size_t pageSize = 1024;

class SparseSet {

private:

public:
    size_t componentSize; //stride
    int sparse[maxEntities] {};
    std::vector<Entity> denseEntities {};
    std::vector<std::byte> denseComponents {};
    
    SparseSet() : componentSize(0) {} // default constructor 
    SparseSet(size_t size) : componentSize(size) {}

    size_t size() const;

    bool hasEntity(Entity entity) const; //checks if the sparse set contains an entity
    
    void write(Entity entity, const void* componentSourceBytes);
    void remove(Entity entity);

    // return pointer to component source bytes 
    void* getRaw(Entity entity);

};
