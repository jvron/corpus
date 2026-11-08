#include "corpus/clock.hpp"
#include "corpus/input.hpp"
#include "corpus/world.hpp"
#include "corpus/window.hpp"
#include "corpus/engine.hpp"
#include "corpus/systems.hpp"
#include "corpus/registry.hpp"
#include "corpus/renderer.hpp"
#include "corpus/scheduler.hpp"
#include "corpus/components.hpp"
#include "corpus/resource_manager.hpp"

void Engine::registerComponents(World& world) {

    Registry& registry = world.registry;
    
    registry.registerComponent<Mesh>();
    registry.registerComponent<Material>();
    registry.registerComponent<Color>();
    registry.registerComponent<Renderable>();
    registry.registerComponent<Transform>();
    registry.registerComponent<Camera>();
    registry.registerComponent<CameraController>();
    registry.registerComponent<PointLight>();
    registry.registerComponent<DirectionalLight>();
    registry.registerComponent<Spotlight>();
    registry.registerComponent<Parent>();
    registry.registerComponent<Child>();
    registry.registerComponent<WorldTransform>();
    registry.registerComponent<Name>();
}

void Engine::startup(World& world) {

    Window::create(world);
    Renderer::init(world);
    Input::init(world);
    Clock::init(world);

    scheduler.init(world);
    world.resourceManager.init();
    
    registerComponents(world);

    scheduler.addSystem(Stage::Begin, Window::pollEvents);

    scheduler.addSystem(Stage::Update, CameraControlSystem::update);
    scheduler.addSystem(Stage::Update, TransformSystem::update);

    scheduler.addSystem(Stage::Render, Renderer::beginFrame);
    scheduler.addSystem(Stage::Render, Renderer::renderScene);

    scheduler.addSystem(Stage::End, Window::swapBuffers);
    scheduler.addSystem(Stage::End, Input::resetInputState);
}

void Engine::runFrame(World& world) {

    Clock::tick(world);

    scheduler.runStage(Stage::Begin);
    scheduler.runStage(Stage::Input);
    scheduler.runStage(Stage::Update);
    scheduler.runStage(Stage::Render);
    scheduler.runStage(Stage::End);
};

void Engine::run(World& world) {

    scheduler.runStage(Stage::Startup);

    while (!Window::shouldClose(world)) {
        
        runFrame(world);
    }

    scheduler.runStage(Stage::Shutdown);
}

void Engine::shutdown(World& world) {

    world.resourceManager.destroy();
    Window::destroy(world);
}