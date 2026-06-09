#include "engine/engine.hpp"
#include "engine/input.hpp"
#include "engine/world.hpp"
#include "engine/scheduler.hpp"
#include "platform/window/window.hpp"

void Engine::startUp(World &world) {

    Window::create(world);
    Input::init(world);

    scheduler.init(world);
    //Renderer::init(world);

    scheduler.addSystem(Stage::Begin, Window::pollEvents);
    scheduler.addSystem(Stage::Update, Input::resetKeyStates);
    //scheduler.addSystem(Stage::Render, Renderer::beginFrame);
    //scheduler.addSystem(Stage::Render, Renderer::renderScene);
    //scheduler.addSystem(Stage::Render, Renderer::endFrame);
    scheduler.addSystem(Stage::End, Window::swapBuffers);

}

void Engine::shutDown(World &world) {

    Window::destroy(world);
}

void Engine::run(World &world) {

    while (!Window::shouldClose(world)) {
        scheduler.runStage(Stage::Begin);
        scheduler.runStage(Stage::Input);
        scheduler.runStage(Stage::Update);
        //scheduler.runStage(Stage::Render);
        scheduler.runStage(Stage::End);
    }
}