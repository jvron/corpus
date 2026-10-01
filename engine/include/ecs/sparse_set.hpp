#pragma once

#include <vector>

#include "ecs/components.hpp"

constexpr int maxEntities = 1024;
//constexpr size_t pageSize = 1024;

class SparseSet {

public:
    size_t componentSize {}; //stride
    int sparse[maxEntities] {};
    std::vector<Entity> denseEntities;
    std::vector<std::byte> denseComponents;
    
    SparseSet() : componentSize(0) {}
    SparseSet(size_t size) : componentSize(size) {}

    size_t size() const;
    bool hasEntity(Entity entity) const;
    
    void write(Entity entity, const void* componentSourceBytes);
    void remove(Entity entity);

    // return pointer to component source bytes 
    void* getRaw(Entity entity);
    void* getRawUnsafe(Entity entity); 
};
