#include "engine/engine.hpp"
#include "engine/input.hpp"
#include "engine/registry.hpp"
#include "engine/scheduler.hpp"
#include "platform/window/window.hpp"
#include "renderer/renderer.hpp"

void Engine::startUp(Registry &registry) {

    Window::create(registry);
    Input::init(registry);

    //Scheduler scheduler;
    scheduler.init(registry);

    scheduler.addSystem(Stage::Window, Window::pollEvents);
    scheduler.addSystem(Stage::Window, Window::swapBuffers);
    scheduler.addSystem(Stage::Update, Input::resetKeyStates);

}

void Engine::shutDown(Registry &registry) {

    Window::destroy(registry);
}

void Engine::run(Registry &registry) {

    while (!Window::shouldClose(registry)) {
        scheduler.runStage(Stage::Window);
        scheduler.runStage(Stage::Input);
        scheduler.runStage(Stage::Update);
    }
}