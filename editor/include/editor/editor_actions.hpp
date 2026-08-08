#pragma once 

#include <string_view>

struct World;
struct Entity;

namespace EditorActions {

    Entity createEntity(World& world, std::string_view name);
    void deleteEntity(World& world, Entity entity);
}