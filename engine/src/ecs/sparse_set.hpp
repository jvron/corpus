#pragma once

#include <vector>

#include "engine/components.hpp"

constexpr int maxEntities = 100;

template<typename Component>

class SparseSet {

    public:
    int sparseArray[maxEntities] {};
    std::vector<Component> denseArray {};
    std::vector<Entity> entityArray {};

    void insert(const Component &component, const Entity &entity);
    Component* lookup(const Entity &entity);
    void detete(const Entity &entity);
};