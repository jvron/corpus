#pragma once 

#include <array>
#include <cstdint>
#include <tuple>
#include <vector>

#include "ecs/components.hpp"
#include "ecs/registry.hpp"
#include "ecs/sparse_set.hpp"

template <typename ... Component>
class View {

private:
    std::array<SparseSet*, sizeof...(Component)> poolPtrs;
    SparseSet& leaderPool;
    Registry& registryRef;

public:

    View(Registry &registry) : poolPtrs{registry.getPool<Component>()...}, leaderPool(getLeaderPool()), registryRef(registry) {};

    class Iterator {

    private:
        void validateOrAdvance() {
            if (index >= pool.size()) {
                return;
            }

            while (index < pool.size()) {

                Entity entity = pool.denseEntities[index];
                bool isMatch = true;

                for (uint32_t i = 0; i < sizeof...(Component); i++) {

                    if (!view->poolPtrs[i]->hasEntity(entity)) {
                        isMatch = false;
                        break;
                    }
                }
                if (!isMatch) {
                    index++;
                }
                else {
                    return;
                }
            }
        }

    public:
        View* view;
        SparseSet& pool;
        uint32_t index {}; // current dense index

        Iterator(View& v, SparseSet& p, uint32_t i) : view(&v), pool(p), index(i) {
            validateOrAdvance();
        }

        std::tuple<Entity, Component& ...> operator*() {
            Entity entity = pool.denseEntities[index];
            std::tuple<Entity, Component& ...> tup(entity, view->registryRef.template getComponent<Component>(entity) ...);
            return tup;
        }

        bool operator!=(const Iterator& other) {
            if (index != other.index) {
                return true;
            }
            return false;
        }

        Iterator& operator++() {
            index++;
            validateOrAdvance();
            return *this;
        }
    };

    Iterator begin() {
        return Iterator(*this, leaderPool, 0);
    }

    Iterator end() {
        return Iterator(*this, leaderPool, leaderPool.size());
    }

    SparseSet& getLeaderPool() {

        int leaderIndex = 0;
        for (uint32_t i = 0; i < sizeof...(Component); i++) {
            if (poolPtrs[i]->size() < poolPtrs[leaderIndex]->size()) {
                leaderIndex = i;
            }
        }
        return *poolPtrs[leaderIndex];
    }
};