#include <cstdio>

#include "engine/engine.hpp"
#include "ecs/components.hpp"
#include "engine/systems.hpp"
#include "platform/clock.hpp"
#include "platform/input.hpp"
#include "engine/world.hpp"
#include "engine/scheduler.hpp"
#include "renderer/renderer.hpp"
#include "platform/window.hpp"
#include "resources/resource_manager.hpp"

void Engine::registerComponents(World& world) {
    world.registry.registerComponent<Mesh>();
    world.registry.registerComponent<Material>();
    world.registry.registerComponent<Color>();
    world.registry.registerComponent<Renderable>();
    world.registry.registerComponent<Transform>();
    world.registry.registerComponent<Camera>();
    world.registry.registerComponent<CameraController>();
    world.registry.registerComponent<PointLight>();
    world.registry.registerComponent<DirectionalLight>();
    world.registry.registerComponent<Spotlight>();
    world.registry.registerComponent<Parent>();
    world.registry.registerComponent<Child>();

}

void Engine::startUp(World &world) {

    Window::create(world);
    Renderer::init(world);
    Input::init(world);
    Clock::init(world);

    scheduler.init(world);
    world.resourceManager.init();
    
    registerComponents(world);

    scheduler.addSystem(Stage::Begin, Window::pollEvents);
    scheduler.addSystem(Stage::Update, CameraControlSystem::update);
    scheduler.addSystem(Stage::Update, Input::resetInputState);
    scheduler.addSystem(Stage::Render, Renderer::beginFrame);
    scheduler.addSystem(Stage::Render, Renderer::renderScene);
    scheduler.addSystem(Stage::End, Window::swapBuffers);
}

void Engine::run(World &world) {

    while (!Window::shouldClose(world)) {

        Clock::tick(world);

        scheduler.runStage(Stage::Begin);
        scheduler.runStage(Stage::Input);
        scheduler.runStage(Stage::Update);
        scheduler.runStage(Stage::Render);
        scheduler.runStage(Stage::End);
    }
}

void Engine::shutDown(World& world) {

    world.resourceManager.destroy();
    Window::destroy(world);
}