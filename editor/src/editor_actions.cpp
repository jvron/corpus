#include <string_view>

#include "core/string_pool.hpp"
#include "ecs/components.hpp"
#include "engine/world.hpp"

#include "editor/editor_actions.hpp"

Entity EditorActions::createEntity(World& world, std::string_view name) {

    Entity entity = world.registry.createEntity();

    StringID nameId = world.stringPool.intern(name);

    world.registry.insert(entity, Name {nameId});

    return entity;
}

void EditorActions::deleteEntity(World& world, Entity entity) {

    world.registry.destroyEntity(entity);
}