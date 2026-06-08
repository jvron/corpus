#pragma once

#include "ecs/components.hpp"
#include "ecs/sparse_set.hpp"
#include <cassert>
#include <cstdint>
#include <sys/types.h>
#include <vector>


constexpr int32_t keyCount = 348; 
struct GLFWwindow;

struct WindowConfig { // user defined
    int width {};
    int height {};
    const char* title = nullptr;
};

struct WindowState {
    GLFWwindow* handle = nullptr;
};

struct InputState {
    
    // per frame key state (reset per frame)
    bool keyPressed[keyCount] {};
    bool keyReleased[keyCount] {};

   // persistent key state 
    bool keyDown[keyCount] {};
};

struct RenderState {
    Color clearColor;
};

struct Resources {

    WindowConfig windowConfig;
    WindowState windowState;
    InputState inputState;
    RenderState renderState;
};

class Registry {

private:
    static inline int typeCounter = 0;

public:

    std::vector<DynamicSparseSet> pools;

    template<typename T>
    static uint32_t getId() { //unique type id for each component type, a new function is created for each component type
        static uint32_t typeId = typeCounter++; //typeCounter is only incremented once (first function call)
        return typeId;
    }

    template <typename T>
    DynamicSparseSet* getPool() {
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

        DynamicSparseSet pool = DynamicSparseSet(sizeof(T));
        uint32_t componentId = getId<T>(); 

        if (componentId >= pools.size()) {
            pools.resize(componentId + 1);
        }
        pools[componentId] = pool;
    }

    template <typename T>
    T& getComponent(const Entity &entity) { //assumes that the entity exists (to be used by view and not by the game)

        DynamicSparseSet* pool = getPool<T>();
        assert(pool == nullptr && "Error: requested pool does not exist");
        return *static_cast<T*>(pool->getRaw(entity));
    } 
};