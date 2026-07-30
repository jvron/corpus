#pragma once 

#include <array>
#include <cstdint>
#include <tuple>
#include <vector>

#include "ecs/registry.hpp"
#include "ecs/sparse_set.hpp"

template <typename ... Component>
class View {

public:
    View(Registry &registry) : poolPtrs{registry.getPool<Component>()...}, registryRef(registry), leaderPool(getLeaderPool()) {};

    SparseSet& getLeaderPool() {

        int leaderIndex = 0;
        for (uint32_t i = 0; i < sizeof...(Component); i++) {
    
            assert(poolPtrs[i] && "[ERROR]:  Component type must be registered via registerComponent<T>() before requesting a view");
            
            if (poolPtrs[i]->size() < poolPtrs[leaderIndex]->size()) {
                leaderIndex = i;
            }
        }
        return *poolPtrs[leaderIndex];
    }

    class Iterator {

    public:
        Iterator(View& v, SparseSet& p, uint32_t i) : view(v), pool(p), index(i) {
            validateOrAdvance();
        }

        std::tuple<Entity, Component& ...> operator*() {
            Entity entity = pool.denseEntities[index];
            return {entity, view.registryRef.template getComponentUnsafe<Component>(entity) ...} ;
        }

        bool operator!=(const Iterator& other) {
            return index != other.index;
        }

        Iterator& operator++() {
            index++;
            validateOrAdvance();
            return *this;
        }
        
    private:
        View& view;
        SparseSet& pool;
        uint32_t index {}; // current dense index

        void validateOrAdvance() {
            if (index >= pool.size()) {
                return;
            }

            while (index < pool.size()) {

                Entity entity = pool.denseEntities[index];
                bool isMatch = true;

                for (uint32_t i = 0; i < sizeof...(Component); i++) {

                    if (!view.poolPtrs[i]->hasEntity(entity)) {
                        isMatch = false;
                        break;
                    }
                }

                if (isMatch) {
                    return;
                }
                index++;
            }
        }
    };

    Iterator begin() {
        return Iterator(*this, leaderPool, 0);
    }

    Iterator end() {
        return Iterator(*this, leaderPool, leaderPool.size());
    }

private:
    std::array<SparseSet*, sizeof...(Component)> poolPtrs;
    Registry& registryRef;
    SparseSet& leaderPool;
};