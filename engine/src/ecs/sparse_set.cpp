#include "sparse_set.hpp"
#include "engine/components.hpp"
#include <cstddef>

template<typename Component> 
void SparseSet<Component>::insert(const Component &component,const Entity &entity) {

    const size_t denseArraySize = denseArray.size();

    denseArray.emplace(denseArray.begin() + denseArraySize, component);
    entityArray.emplace(entityArray.begin() + entityArray.size(), entity);

    sparseArray[entity.id] = denseArraySize;
}

template <typename Component>
Component* SparseSet<Component>::lookup(const Entity &entity) {

    int index = sparseArray[entity.id];

    if (index >= denseArray.size() || index < 0) {
        return nullptr;
    }
    return &denseArray[index]; 
    
}
template<typename Component>

void SparseSet<Component>::detete(const Entity &entity) {

    int index = sparseArray[entity.id];
    
    if (index >= denseArray.size() || index < 0) {
        return;
    }

    const size_t denseArraySize = denseArray.size();
    const size_t entityArraySize = entityArray.size();

    denseArray[index] = denseArray[denseArraySize - 1];
    entityArray[index] = entityArray[entityArraySize - 1];
    
    denseArray.pop_back();
    entityArray.pop_back();

    sparseArray[entity.id] = -1; // mark as invalid, will be reused

    sparseArray[entityArray[index].id] = index; //assign index to the moved enitity
}