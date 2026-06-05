#pragma once

#include <vector>

#include "ecs/components.hpp"

constexpr int maxEntities = 100;
//constexpr size_t pageSize = 1024;

template<typename Component>

class SparseSet {

    public:
    int sparseArray[maxEntities] {};
    std::vector<Component> denseArray {};
    std::vector<Entity> entityArray {};

    void insert(const Component &component, const Entity &entity);
    Component* lookup(const Entity &entity);
    void remove(const Entity &entity);
};

#include "sparse_set.tpp"