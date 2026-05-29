#include "engine/engine.hpp"
#include "platform/window/window.hpp"
#include "renderer/renderer.hpp"

void Engine::run() {

    Window window;
    Renderer renderer;

    window.create(620, 420, "test");
    renderer.init();

    while (!window.shouldClose()) {
        window.swapBuffers();
        window.pollEvents();
    }
    

    window.destroy();
}