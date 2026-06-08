#pragma once

#include "ecs/components.hpp"
#include "ecs/sparse_set.hpp"
#include <cassert>
#include <cstdint>
#include <sys/types.h>
#include <vector>

class Registry {

private:
    static inline int typeCounter = 0;
    std::vector<SparseSet> pools;

public:

    template<typename T>
    static uint32_t getId() { //unique type id for each component type, a new function is created for each component type
        static uint32_t typeId = typeCounter++; //typeCounter is only incremented once (first function call)
        return typeId;
    }

    template <typename T>
    SparseSet* getPool() {
        
        uint32_t componentId = getId<T>();

        if (componentId >= pools.size() || pools[componentId].componentSize == 0) {
            return nullptr;
        }
        
        return &pools[componentId];
    } 

    template<typename T> 
    void registerComponent() {

        if (getPool<T>() != nullptr) {
            return;
        }

        SparseSet pool = SparseSet(sizeof(T));
        uint32_t componentId = getId<T>(); 

        if (componentId >= pools.size()) {
            pools.resize(componentId + 1);
        }
        pools[componentId] = pool;
    }

    template <typename T>
    T& getComponent(const Entity &entity) { //assumes that the entity exists (to be used by view and not by the game)

        SparseSet* pool = getPool<T>();
        assert(pool == nullptr && "Error: requested pool does not exist");

        return *static_cast<T*>(pool->getRaw(entity));
    } 
};