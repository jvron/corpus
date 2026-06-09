#pragma once 

#include <array>
#include <vector>

#include "ecs/components.hpp"
#include "ecs/registry.hpp"
#include "ecs/sparse_set.hpp"

template <typename ... Component>
class View {

private:
    std::array<SparseSet*, sizeof...(Component)> poolPtrs;

public:

    View(Registry &registry) : poolPtrs{registry.getPool<Component>()...} {};
    SparseSet* getLeaderPool() {

        int leaderIndex = 0;
        for (int i = 0; i < sizeof...(Component); i++) {
            if (poolPtrs[i]->size() < poolPtrs[leaderIndex]->size()) {
                leaderIndex = i;
            }
        }
        return poolPtrs[leaderIndex];
    }

    std::vector<Entity> getEntities() {
        SparseSet* leaderPool = getLeaderPool();

        std::vector<Entity> validEntities;

        for (int i = 0; i < leaderPool->denseEntities.size(); i++) {

            Entity entity = leaderPool->denseEntities[i];
            bool isMatch = true;

            for (int j = 0; j < sizeof...(Component); j++) {

                if (!poolPtrs[j]->hasEntity(entity)) {
                    isMatch = false;
                }
            }

            if (isMatch) {
                validEntities.push_back(entity);
            }
        }
        return validEntities;
    }
};