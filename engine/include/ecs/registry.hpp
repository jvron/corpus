#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <span>
#include <sys/types.h>
#include <vector>

#include "ecs/components.hpp"
#include "ecs/sparse_set.hpp"

class Registry {

public:
    Entity createEntity();
    bool isValidEntity(Entity entity);
    void destroyEntity(Entity entity);

    std::span<const Entity> entities() const;
    size_t entityCount() const;

    template<typename T> //unique type id for each component type, a new function is created for each component type
    static uint32_t getId() {
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
    T& getComponent(Entity entity) { 

        SparseSet* pool = getPool<T>();
        assert(pool != nullptr && "[ERROR]: Requested pool does not exist");

        T* component = static_cast<T*>(pool->getRaw(entity));
        assert(component != nullptr && "[ERROR]: Invalid entity provided");

        return *component;
    }

    template <typename T>
    T& getComponentUnsafe(Entity entity) { //assumes that the entity exists (to be used by view and not by the game)

        SparseSet* pool = getPool<T>();
        assert(pool != nullptr && "[ERROR]: Requested pool does not exist");

        return *static_cast<T*>(pool->getRawUnsafe(entity));
    }

    template <typename T>
    void insert(Entity entity, const T& component) {

        assert(getPool<T>() != nullptr && "[ERROR]: Component type must be registered via registerComponent<T>() before insertion");
        uint32_t componentId = getId<T>();

        pools[componentId].write(entity, &component);
    }

    template <typename ... T>
    void insertComponents(Entity entity, const T&... components) {

        static_assert(sizeof...(T) > 0, "[ERROR]: Must provide atleast one component");

        (insert(entity, components), ...);
    }

    template <typename T> 
    void remove(Entity entity) {

        assert(getPool<T>() != nullptr && "[ERROR]: Component provided does not exist");
        uint32_t componentId = getId<T>();

        pools[componentId].remove(entity);
    };

    template <typename ...T> 
    void removeComponents(Entity entity) {

        static_assert(sizeof...(T) > 0, "[ERROR]: Must provide atleast one component");

        (remove<T>(entity), ...);
    }

    template<typename  T> 
    bool hasComponent(Entity entity) {

        const SparseSet* pool = getPool<T>();
        assert(pool && "[ERROR]: Requested pool does not exist");
        
        return pool->hasEntity(entity);
    }
    
private:
    static inline int typeCounter = 0;
    std::vector<SparseSet> pools;
    
    // [EntityID] = generation
    std::vector<uint32_t> generations;
    std::vector<EntityID> freeEntityIDs; 

    // [EntityID] = activeEntities index
    std::vector<size_t> entityLocations; 
    std::vector<Entity> activeEntities;
};
